#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
/// OptiX
#include <optixu/optixpp_namespace.h>
/// Logging
#include <plog/Log.h>
/// Fusion
#include <scene/scene.h>
#include <ptx_compiler/ptx_compiler.h>
#include <optix_scene_hierarchy/scene_root.h>
#include <optix_rendering_system/solid_color_program.h>
#include <optix_rendering_system/pinhole_cam_program.h>
#include <optix_rendering_system/simple_miss_program.h>
#include <optix_rendering_system/exception_program.h>
#include <optix_rendering_system/bounding_box_program.h>
#include <optix_rendering_system/intersection_program.h>
#include <optix_rendering_system/closest_hit_program.h>
#include <optix_rendering_system/any_hit_program.h>
/// STL
#include <vector>
#include <functional>
#include <memory>
namespace tree {
	/**
	*	Scene Graph
	*/
	class SceneTree {
	public:
		explicit SceneTree() { }
		explicit SceneTree(optix::Context& ctx)
			: mContext(ctx)
		{ }
		explicit SceneTree(
			optix::Context& ctx, 
			const std::shared_ptr<ptx::PTXCompiler> compiler,
			const std::size_t& width,
			const std::size_t& height);
		const std::size_t rootCount() const { return mRoots.size(); }
		const std::vector<SceneRoot> roots() const { return mRoots; }
		const SceneRoot root(const std::size_t& idx) const;
		void addRoot(const SceneRoot& root);
		std::function<void(scene::Scene& s)> sceneFlowIn();
	protected:
		void addRoot(scene::Scene& s);
		GeometryComponent newGeometryComponent(const scene::Mesh& mesh);
		MaterialComponent newMaterialComponent(const scene::Material& mat);
		optix::TextureSampler newTextureSampler(const scene::Material& mat);
	private:
		optix::Context mContext;
		optix::Group mGroup;			///< This is the tree (top node)
		optix::Acceleration mGroupAcceleration;
		std::vector<SceneRoot> mRoots;
		std::size_t mEntryPointCount;
		std::size_t mRayTypeCount;
		std::shared_ptr<rt::BoundingBoxProgram> mBoundingBoxProgram;
		std::shared_ptr<rt::IntersectionProgram> mIntersectionProgram;
		std::shared_ptr<rt::RTProgram> mClosestHitProgram;
		std::shared_ptr<rt::RTProgram> mAnyHitProgram;
		std::shared_ptr<rt::SolidColorProgram> mSolidColorProgram;
		std::shared_ptr<rt::PinholeCameraProgram> mPinholeProgram;
		std::shared_ptr<rt::SimpleMissProgram> mSimpleMissProgram;
		std::shared_ptr<rt::ExceptionProgram> mExceptionProgram;
		// who has the lights? probaly this guy here.
		// Should the tree have the mapper?
		// It would be cleaner
	};

	/**
	*	Constructor
	*/
	SceneTree::SceneTree(
		optix::Context& ctx,
		const std::shared_ptr<ptx::PTXCompiler> compiler, 
		const std::size_t& width,
		const std::size_t& height)
		: mContext(ctx)
	{
		mBoundingBoxProgram =
			std::make_shared<rt::BoundingBoxProgram>(
				ctx,
				std::string("mesh_bounds"),
				std::string("res\\ptx\\triangle_mesh.ptx"),
				std::string("..\\src\\CUDA\\triangle_mesh.cu"),
				compiler);
		mIntersectionProgram =
			std::make_shared<rt::IntersectionProgram>(
				ctx,
				std::string("mesh_intersect"),
				std::string("res\\ptx\\triangle_mesh.ptx"),
				std::string("..\\src\\CUDA\\triangle_mesh.cu"),
				compiler);
		mClosestHitProgram =
			std::make_shared<rt::RTProgram>(
				ctx,
				std::string("closest_hit_radiance"),
				std::string("res\\ptx\\phong.ptx"),
				std::string("..\\src\\CUDA\\phong.cu"),
				compiler);
		mAnyHitProgram = 
			std::make_shared<rt::RTProgram>(
				ctx,
				std::string("any_hit_shadow"),
				std::string("res\\ptx\\phong.ptx"),
				std::string("..\\src\\CUDA\\phong.cu"),
				compiler);
		mGroup = mContext->createGroup();
		mGroupAcceleration = mContext->createAcceleration("Trbvh");
		mGroup->setAcceleration(mGroupAcceleration);

		mSolidColorProgram = std::make_shared<rt::SolidColorProgram>(
			mContext,
			std::string("draw_solid_color"),
			std::string("\\res\\ptx\\draw_color.ptx"),
			std::string("..\\src\\CUDA\\draw_color.cu"),
			optix::make_float3(0.2f, 0.2f, 0.2f),
			compiler);
		/// Simple Miss Program
		mSimpleMissProgram = std::make_shared<rt::SimpleMissProgram>(
			mContext,
			std::string("miss"),
			std::string("\\res\\ptx\\simple_miss.ptx"),
			std::string("..\\src\\CUDA\\constantbg.cu"),
			optix::make_float3(0.0f, 0.8f, 0.8f),
			compiler);
		/// Exception Program
		mExceptionProgram = std::make_shared<rt::ExceptionProgram>(
			mContext,
			std::string("exception"),
			std::string("\\res\\ptx\\exception.ptx"),
			std::string("..\\src\\CUDA\\exception.cu"),
			optix::make_float3(1.0f, 0.0f, 0.0f),
			compiler);
		/// Pinhole Camera Program
		mPinholeProgram =
			std::make_shared<rt::PinholeCameraProgram>(
				mContext,
				std::string("pinhole_camera"),
				std::string("\\res\\ptx\\pinhole_camera.ptx"),
				std::string("..\\src\\CUDA\\pinhole_camera.cu"),
				compiler,
				width, height);
	}

	/**
	*	adds root
	*/
	void SceneTree::addRoot(const SceneRoot& root) {
		mRoots.emplace_back(root);
	}

	/**
	*	returns indexed root
	*/
	const SceneRoot SceneTree::root(const std::size_t& idx) const {
		if (mRoots.empty()) {
			LOG_ERROR << "Tree has no roots.";
			return SceneRoot();
		}
		else if (idx < 0 || idx >= mRoots.size()) {
			LOG_ERROR << "Index out of range.";
			return SceneRoot();
		}
		return mRoots[idx];
	}

	/**
	*	maps fusion scene to root
	*	(adds a root)
	*/
	std::function<void(scene::Scene& s)> SceneTree::sceneFlowIn() {
		return [this](scene::Scene& s) {
			addRoot(s);
		};
	}

	/**
	*	adds geometry component	
	*/
	GeometryComponent SceneTree::newGeometryComponent(const scene::Mesh& mesh) {
		/****************************************
			Create OptiX Geometry ( & Component)
		*****************************************/
		GeometryComponent geometryComponent(mContext->createGeometry());
		geometryComponent.setPrimitiveCount(mesh.faceCount());
		/*******************
			Create Buffers
		********************/
		optix::Buffer vertexBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT3, mesh.vertexCount());
		optix::Buffer indexBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_INT3, mesh.faceCount());
		optix::Buffer normalsBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT3, mesh.normalsCount());
		optix::Buffer texcoordsBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT2, mesh.texCoordsCount());
		optix::Buffer materialBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_INT, mesh.faceCount());

		/********************
			Map Buffers
		********************/
		// vertex data
		std::memcpy(reinterpret_cast<float*>(vertexBuffer->map()),
			mesh.vertexPositions().data(), mesh.vertexCount() * sizeof(optix::float3));
		// face index data
		std::memcpy(reinterpret_cast<int*>(indexBuffer->map()),
			mesh.faceIndices().data(), mesh.faceCount() * sizeof(optix::uint3));
		// normals data
		std::memcpy(reinterpret_cast<float*>(normalsBuffer->map()),
			mesh.normals().data(), mesh.normalsCount() * sizeof(optix::float3));
		// texcoords data
		std::memcpy(reinterpret_cast<float*>(texcoordsBuffer->map()),
			mesh.texCoords().data(), mesh.texCoordsCount() * sizeof(optix::float2));
		// materials data
		std::memcpy(reinterpret_cast<int*>(materialBuffer->map()),
			mesh.materialIndices().data(), mesh.faceCount() * sizeof(optix::uint));

		/*****************
			Set Programs
		******************/
		try {
			geometryComponent.setBoundingBoxProgram(mBoundingBoxProgram->newProgram());
			geometryComponent.setIntersectionProgram(mIntersectionProgram->newProgram());
		}
		catch (optix::Exception& ex) {
			LOG_ERROR << ex.getErrorCode() << ": " << ex.getErrorString();
		}

		/*****************************
			Set Geometry's Buffer
		*****************************/
		geometryComponent.setVertexBuffer(vertexBuffer);
		geometryComponent.setIndexBuffer(indexBuffer);
		geometryComponent.setNormalsBuffer(normalsBuffer);
		geometryComponent.setTexcoordsBuffer(texcoordsBuffer);
		geometryComponent.setMaterialsBuffer(materialBuffer);

		/*******************
			Unmap Buffers
		********************/
		vertexBuffer->unmap();
		indexBuffer->unmap();
		normalsBuffer->unmap();
		texcoordsBuffer->unmap();
		materialBuffer->unmap();

		/***************************
			Add Geometry Component
		***************************/
		return geometryComponent;
	}

	/**
	*	Adds material Component
	*/
	MaterialComponent SceneTree::newMaterialComponent(const scene::Material& mat) {
		/******************************
			Create Material Component
		*******************************/
		MaterialComponent materialComponent(mContext->createMaterial());

		/***********************
			Set Programs
		***********************/
		materialComponent.setClosestHitProgram(mClosestHitProgram->newProgram());
		materialComponent.setAnyHitProgram(mAnyHitProgram->newProgram());

		/*************************
			Material Parameters
		**************************/
		materialComponent.setDiffusionColor(mat.diffusion());
		materialComponent.setRandianceColor(mat.radiance());
		materialComponent.setSpecularColor(mat.specular());
		materialComponent.setAmbientColor(mat.ambient());
		materialComponent.setShininess(mat.shininess());
		materialComponent.setTextureSampler(newTextureSampler(mat));
		return materialComponent;
	}

	/**
	*	creates new Texture Sampler
	*/
	optix::TextureSampler SceneTree::newTextureSampler(const scene::Material & mat) {
		optix::TextureSampler texSampler = mContext->createTextureSampler();
		texSampler->setWrapMode(0, RT_WRAP_REPEAT);
		texSampler->setWrapMode(1, RT_WRAP_REPEAT);
		texSampler->setWrapMode(2, RT_WRAP_REPEAT);
		texSampler->setIndexingMode(RT_TEXTURE_INDEX_NORMALIZED_COORDINATES);
		texSampler->setReadMode(RT_TEXTURE_READ_NORMALIZED_FLOAT);
		texSampler->setMaxAnisotropy(1.0f);
		texSampler->setMipLevelCount(1u);
		texSampler->setArraySize(1u);

		// buffer with single texel
		// TODO: load textures :)
		optix::Buffer texBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_UNSIGNED_BYTE4, 1u, 1u);
		unsigned char* bufferData = static_cast<unsigned char*>(texBuffer->map());
		bufferData[0] = static_cast<unsigned char>(optix::clamp((int)(mat.diffusion().x * 255.0f), 0, 255));
		bufferData[1] = static_cast<unsigned char>(optix::clamp((int)(mat.diffusion().y * 255.0f), 0, 255));
		bufferData[2] = static_cast<unsigned char>(optix::clamp((int)(mat.diffusion().z * 255.0f), 0, 255));
		bufferData[3] = 255;
		texBuffer->unmap();
		texSampler->setBuffer(0u, 0u, texBuffer);
		texSampler->setFilteringModes(RT_FILTER_LINEAR, RT_FILTER_LINEAR, RT_FILTER_NONE);
		return texSampler;
	}

	/**
	*	maps fusion scene to root
	*/
	// TODO: versatile scene adding
	void SceneTree::addRoot(scene::Scene& s) {
		// create a root
		SceneRoot root;
		// compress scene to one mesh
		LOG_DEBUG << "Compressing Scene...";
		s.compress();
		scene::Mesh m = s.meshes().back();
		LOG_DEBUG << "Getting Materials...";
		// get materials
		std::vector<scene::Material> mats = s.materials();

		LOG_DEBUG << "Creating Transform Component...";
		TransformComponent transformComponent(mContext->createTransform());
		mGroup->addChild(transformComponent.get());
		LOG_DEBUG << "Creating Geometry Group Component...";
		GeometryGroupComponent ggroupComponent(mContext->createGeometryGroup());
		ggroupComponent.setAcceleration(mContext->createAcceleration("Trbvh"));
		LOG_DEBUG << "Creating Geometry Instance Component...";
		GeometryInstanceComponent ginstanceComponent(mContext->createGeometryInstance());
		LOG_DEBUG << "Creating Material Components...";
		for (std::vector<scene::Material>::const_iterator it = mats.begin();
			it != mats.end(); ++it) {
			ginstanceComponent.addMaterialComponent(newMaterialComponent(*it));
		}
		LOG_DEBUG << "Creating Geometry Component...";
		ginstanceComponent.addGeometryComponent(newGeometryComponent(m));
		ggroupComponent.setGinstanceComponent(ginstanceComponent);
		transformComponent.addGeometryGroupComponent(ggroupComponent);
		root.addTransformComp(transformComponent);
		mRoots.emplace_back(root);
		mContext["top_object"]->set(mGroup);
		mContext["top_shadower"]->set(mGroup);
	}

}	//	!namespace tree
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
