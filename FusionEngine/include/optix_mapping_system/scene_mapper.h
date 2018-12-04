#ifndef INCLUDE_OPTIX_MAPPING_SYSTEM_SCENE_MAPPER_H
#define INCLUDE_OPTIX_MAPPING_SYSTEM_SCENE_MAPPER_H

#include <plog/Log.h>

#include <optix_mapping_system/geometry_mapper.h>
#include <optix_mapping_system/geometry_instance_mapper.h>

#include <scene/scene.h>

#include <memory>

namespace map {
	/** 
	*	Maps Fusion's scene to OptiX Traversal Graph
	*/
	class SceneMapper {
	public:
		explicit SceneMapper(optix::Context& ctx);
		void map(const scene::Scene& s);
	protected:
		void compressScene(const scene::Scene& s);
	private:
		optix::Context mContext;
		optix::Group mGroupNode;
		std::vector<optix::Transform> mTransformNodes;
		std::shared_ptr<GeometryMapper> mGeometryMapper;
		std::shared_ptr<GinstanceMapper> mGinstanceMapper;
	};

	/**
	*	Constructor
	*/
	SceneMapper::SceneMapper(optix::Context& ctx) {
		mContext = ctx;
		mGroupNode = mContext->createGroup();
		mGeometryMapper = std::make_shared<GeometryMapper>(ctx);
		mGinstanceMapper = std::make_shared<GinstanceMapper>(ctx);
	}

	/**
	*	Compresses Scene to a single Mesh
	*/
	void SceneMapper::compressScene(const scene::Scene& s) {
		std::vector<optix::float3> vertexPositions;
		std::vector<optix::uint3> facesIndices;
		std::vector<optix::float3> normals;
		std::vector<optix::float2> texcoords;
		std::vector<optix::uint> materialIndices;

		LOG_DEBUG << "Compressing scene...";
		/// mesh loop
		for (std::size_t m = 0; m < s.meshCount(); ++m) {
			scene::Mesh mesh = s.meshes()[m];
			/// faces loop
			for (std::size_t f = 0; f < mesh.faceCount(); ++f) {
				facesIndices.emplace_back(mesh.faceIndices()[f]);
			}
			/// materials loop
			for (std::size_t mat = 0; mat < mesh.materialsCount(); ++mat) {
				materialIndices.emplace_back(mesh.materialIndices()[mat]);
			}
			/// vertex loop
			for (std::size_t v = 0; v < mesh.vertexCount(); ++v) {
				vertexPositions.emplace_back(mesh.vertexPositions()[v]);
				if (mesh.hasNormals()) {
					normals.emplace_back(mesh.normals()[v]);
				}
				if (mesh.hasTexCoords()) {
					texcoords.emplace_back(mesh.texCoords()[v]);
				}
			}	//	!vertex loop
		}	//	!mesh loop

		LOG_DEBUG << "Done compressing scene to a single mesh.";
		LOG_DEBUG << "Num of vertices: " << vertexPositions.size();
		LOG_DEBUG << "Num of faces: " << facesIndices.size();
		LOG_DEBUG << "Num of materials: " << materialIndices.size();
		LOG_DEBUG << "Num of normals: " << normals.size();
		LOG_DEBUG << "Num of texcoords: " << texcoords.size();
	}

	/**
	*	Maps scene to OptiX
	*/
	void SceneMapper::map(const scene::Scene& s) {
		for (std::size_t mesh = 0; mesh < s.meshCount(); ++mesh) {
			optix::Geometry geom = mGeometryMapper->map(s.meshes()[mesh]);
			optix::GeometryInstance ginstance = mGinstanceMapper->map(geom, s.materials());
			optix::GeometryGroup ggroup = mContext->createGeometryGroup();
			ggroup->setAcceleration(mContext->createAcceleration("Trbvh"));
			ggroup->addChild(ginstance);
			optix::Transform transform = mContext->createTransform();
			transform->setChild(ggroup);
			mGroupNode->addChild(transform);
		}
	}
}
#endif // !INCLUDE_OPTIX_MAPPING_SYSTEM_SCENE_MAPPER_H

