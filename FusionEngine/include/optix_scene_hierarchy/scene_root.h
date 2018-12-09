#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_ROOT_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_ROOT_H
/// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
/// Fusion
#include <scene/scene.h>
#include <scene/basic_light.h>
/// STL
#include <vector>
#include <memory>
namespace tree {
	/**
	*	Struct to hold geometry's buffers
	*/
	struct GeometryBuffers {
		optix::Buffer vertexBuffer;
		optix::Buffer indexBuffer;
		optix::Buffer normalsBuffer;
		optix::Buffer texcoordsBuffer;
		optix::Buffer materialsBuffer;
	};

	struct Material {
		optix::Material material;
		optix::TextureSampler texSampler;
		optix::Program closestHitProgram;
		optix::Program anyHitProgram;
	};

	/**
	*	Scene Root, holds scene hierarchy
	*/
	class SceneRoot {
	public:
		explicit SceneRoot() { }
		SceneRoot createNew(optix::Context& ctx, scene::Scene& s);
		const optix::Transform topNode() const { return mTransform; }
		optix::Transform& topNode() { return mTransform; }
		void translate(const optix::float3& transVector);
	protected:
		void addBuffers(optix::Context& ctx, const scene::Mesh& mesh);
		void addGeometryPrograms(optix::Context& ctx);
		void addMaterials(optix::Context& ctx, const std::vector<scene::Material>& materials);
		optix::TextureSampler createTextureSampler(optix::Context& ctx, const scene::Material& mat);
		void addDefaultBasicLight(optix::Context& ctx);
	private:
		optix::Matrix4x4 mTransMat;
		optix::Geometry mGeometry;
		GeometryBuffers mGeometryBuffers;
		optix::Program mIntersectionProgram;
		optix::Program mBoundingBoxProgram;
		std::vector<Material> mMaterials;
		optix::GeometryInstance mGinstance;
		optix::GeometryGroup mGGroup;
		optix::Transform mTransform;
		std::vector<scene::BasicLight> mBasicLights;
		optix::Buffer mBasicLightsBuffer;
	};

	/**
	*	Creates geometry's buffers
	*/
	void SceneRoot::addBuffers(optix::Context& ctx, const scene::Mesh& mesh) {
		// create buffers
		mGeometryBuffers.vertexBuffer = ctx->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT3, mesh.vertexCount());
		mGeometryBuffers.indexBuffer = ctx->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_UNSIGNED_INT3, mesh.faceCount());
		mGeometryBuffers.normalsBuffer = ctx->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT3, mesh.normalsCount());
		mGeometryBuffers.texcoordsBuffer = ctx->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT2, mesh.texCoordsCount());
		mGeometryBuffers.materialsBuffer = ctx->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_UNSIGNED_INT, mesh.faceCount());

		// copy data to buffers
		std::memcpy(reinterpret_cast<float*>(mGeometryBuffers.vertexBuffer->map()),
			mesh.vertexPositions().data(), mesh.vertexCount() * sizeof(optix::float3));
		std::memcpy(reinterpret_cast<unsigned int*>(mGeometryBuffers.indexBuffer->map()),
			mesh.faceIndices().data(), mesh.faceCount() * sizeof(optix::uint3));
		std::memcpy(reinterpret_cast<float*>(mGeometryBuffers.normalsBuffer->map()),
			mesh.normals().data(), mesh.normalsCount() * sizeof(optix::float3));
		std::memcpy(reinterpret_cast<float*>(mGeometryBuffers.texcoordsBuffer->map()),
			mesh.texCoords().data(), mesh.texCoordsCount() * sizeof(optix::float2));
		std::memcpy(reinterpret_cast<unsigned int*>(mGeometryBuffers.materialsBuffer->map()),
			mesh.materialIndices().data(), mesh.materialIndices().size() * sizeof(optix::uint));

		// assign buffers to geomtry
		mGeometry["vertex_buffer"]->setBuffer(mGeometryBuffers.vertexBuffer);
		mGeometry["index_buffer"]->setBuffer(mGeometryBuffers.indexBuffer);
		mGeometry["normal_buffer"]->setBuffer(mGeometryBuffers.normalsBuffer);
		mGeometry["texcoord_buffer"]->setBuffer(mGeometryBuffers.texcoordsBuffer);
		mGeometry["material_buffer"]->setBuffer(mGeometryBuffers.materialsBuffer);

		// unmap buffers
		mGeometryBuffers.vertexBuffer->unmap();
		mGeometryBuffers.indexBuffer->unmap();
		mGeometryBuffers.normalsBuffer->unmap();
		mGeometryBuffers.texcoordsBuffer->unmap();
		mGeometryBuffers.materialsBuffer->unmap();
	}

	/**
	*	creates and assigns geometry programs
	*/
	void SceneRoot::addGeometryPrograms(optix::Context& ctx) {
		mIntersectionProgram = ctx->createProgramFromPTXFile("res\\ptx\\triangle_mesh.ptx", "mesh_intersect");
		mBoundingBoxProgram = ctx->createProgramFromPTXFile("res\\ptx\\triangle_mesh.ptx", "mesh_bounds");
		mGeometry->setIntersectionProgram(mIntersectionProgram);
		mGeometry->setBoundingBoxProgram(mBoundingBoxProgram);
	}

	/***
	*	Creates new texturemap
	*/
	optix::TextureSampler SceneRoot::createTextureSampler(optix::Context& ctx, const scene::Material& mat) {
		optix::TextureSampler texSampler = ctx->createTextureSampler();
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
		optix::Buffer texBuffer = ctx->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_UNSIGNED_BYTE4, 1u, 1u);
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
	*	creates and adds materials
	*/
	void SceneRoot::addMaterials(optix::Context& ctx, const std::vector<scene::Material>& materials) {
		Material rootMat;
		for (std::size_t m = 0; m < materials.size(); ++m) {
			rootMat.material = ctx->createMaterial();
			rootMat.closestHitProgram = ctx->createProgramFromPTXFile("res\\ptx\\phong.ptx", "closest_hit_radiance");
			rootMat.anyHitProgram = ctx->createProgramFromPTXFile("res\\ptx\\phong.ptx", "any_hit_shadow");
			rootMat.material->setClosestHitProgram(0u, rootMat.closestHitProgram);
			rootMat.material->setAnyHitProgram(1u, rootMat.anyHitProgram);
			rootMat.texSampler = createTextureSampler(ctx, materials[m]);
			rootMat.material["Kd_map"]->setTextureSampler(rootMat.texSampler);
			rootMat.material["Kd_mapped"]->setInt(0);
			rootMat.material["Kd"]->setFloat(materials[m].diffusion());
			rootMat.material["Kr"]->setFloat(materials[m].radiance());
			rootMat.material["Ks"]->setFloat(materials[m].specular());
			rootMat.material["Ka"]->setFloat(materials[m].ambient());
			rootMat.material["phong_exp"]->setFloat(materials[m].shininess());
			mMaterials.emplace_back(rootMat);
		}
	}

	/**
	*	Creates new Root from fusion Scene
	*/
	SceneRoot SceneRoot::createNew(optix::Context& ctx, scene::Scene& s) {
		// Compress Scene
		// TODO: do not compress scene and make corresponding hierarchy
		s.compress();
		// get materials;
		std::vector<scene::Material> materials = s.materials();

		// make geometry
		mGeometry = ctx->createGeometry();
		scene::Mesh mesh = s.meshes().back();
		addBuffers(ctx, mesh);
		mGeometry->setPrimitiveCount(mesh.faceCount());
		// assign programs to geometry
		addGeometryPrograms(ctx);

		// create geometry instance
		mGinstance = ctx->createGeometryInstance();
		addMaterials(ctx, materials);
		mGinstance->setMaterialCount(mMaterials.size());
		for (std::size_t m = 0; m < mMaterials.size(); ++m) {
			mGinstance->setMaterial(m, mMaterials[m].material);
		}
		mGinstance->setGeometry(mGeometry);

		// create geometry group
		mGGroup = ctx->createGeometryGroup();
		mGGroup->setAcceleration(ctx->createAcceleration("Trbvh"));
		mGGroup->setChildCount(1u);
		mGGroup->setChild(0u, mGinstance);
		
		// create Transform
		mTransform = ctx->createTransform();
		mTransform->setChild(mGGroup);
		mTransMat = optix::Matrix4x4::identity();
		addDefaultBasicLight(ctx);
		return *this;
	}

	void SceneRoot::addDefaultBasicLight(optix::Context& ctx) {
		mBasicLights.emplace_back(
			scene::BasicLight{
			optix::make_float3(-0.5f,  0.25f, -1.0f),
			optix::make_float3(0.2f, 0.2f, 0.25f), 0, 0 });
		mBasicLights.emplace_back(
			scene::BasicLight{
			optix::make_float3(-0.5f, 0.0f, 1.0f),
			optix::make_float3(0.1f, 0.1f, 0.10f), 0, 0 });
		mBasicLights.emplace_back(
			scene::BasicLight{
			optix::make_float3(0.5f, 0.5f, 0.5f),
			optix::make_float3(0.7f, 0.7f, 0.65f), 1, 0 });

		mBasicLightsBuffer = ctx->createBuffer(RT_BUFFER_INPUT);
		mBasicLightsBuffer->setFormat(RT_FORMAT_USER);
		mBasicLightsBuffer->setElementSize(sizeof(scene::BasicLight));
		mBasicLightsBuffer->setSize(mBasicLights.size());
		std::memcpy(mBasicLightsBuffer->map(), mBasicLights.data(), mBasicLights.size() * sizeof(scene::BasicLight));
		mBasicLightsBuffer->unmap();
		ctx["lights"]->set(mBasicLightsBuffer);
	}

	/**
	*	Translates 
	*/
	void SceneRoot::translate(const optix::float3& transVector) {
		mTransMat.translate(transVector);
		mTransform->setMatrix(false, (float*)(&mTransMat), NULL);
		mGGroup->getAcceleration()->markDirty();
	}
	
}
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_ROOT_H

