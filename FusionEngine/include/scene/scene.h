#ifndef INCLUDE_SCENE_SCENE_H
#define INCLUDE_SCENE_SCENE_H
/// OptiX
#include <optixu/optixu_aabb_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
/// Logging
#include <plog/Log.h>
/// Fusion
#include <scene/mesh.h>
#include <scene/material.h>
/// STL
#include <vector>
#include <string>

namespace scene {

	class Scene {
	public:
		explicit Scene() { }
		explicit Scene(const std::size_t& id, const std::string& name);
		const std::size_t meshCount() const { return mMeshes.size(); }
		const std::vector<Mesh> meshes() const { return mMeshes; }
		const std::vector<Material> materials() const { return mMaterials; }
		const optix::Aabb bbox() const { return mBbox; }
		void insertMesh(const Mesh& m);
		void insertMaterial(const Material& m);
		void compress();
		void calcBbox();
	private:
		std::size_t mId;
		std::string mName;
		optix::Aabb mBbox;
		std::vector<Mesh> mMeshes;
		std::vector<Material> mMaterials;
	};

	/*! Construct with id and name
	*/
	Scene::Scene(const std::size_t& id, const std::string& name)
		: mId(id), mName(name)
	{ 
		mBbox.m_min.x = mBbox.m_min.y = mBbox.m_min.z = 1.e16f;
		mBbox.m_max.x = mBbox.m_max.y = mBbox.m_max.z = -1.e16f;
	}

	/*! Places mesh in meshes vector
	*/
	void Scene::insertMesh(const Mesh& m) {
		mMeshes.emplace_back(m);
	}

	/*! Places material in materials vector
	*/
	void Scene::insertMaterial(const Material& m) {
		mMaterials.emplace_back(m);
	}

	/**
	*	calculates scene's bounding box
	*/
	void Scene::calcBbox() {
		for (std::vector<Mesh>::const_iterator m_it = mMeshes.begin();
			m_it != mMeshes.end(); ++m_it) {
			for (std::vector<optix::float3>::const_iterator v_it = m_it->vertexPositions().begin();
				v_it != m_it->vertexPositions().end(); ++v_it) {
				mBbox.m_min.x = std::fminf(mBbox.m_min.x, v_it->x);
				mBbox.m_min.y = std::fminf(mBbox.m_min.y, v_it->y);
				mBbox.m_min.z = std::fminf(mBbox.m_min.z, v_it->z);
				mBbox.m_max.x = std::fmaxf(mBbox.m_max.x, v_it->x);
				mBbox.m_max.y = std::fmaxf(mBbox.m_max.y, v_it->y);
				mBbox.m_max.z = std::fmaxf(mBbox.m_max.z, v_it->z);
			}
		}
	}

	/**
	*	Compresses scene's meshes to one mesh
	*	Without extra triangulation steps
	*/
	void Scene::compress() {
		if (mMeshes.size() == 1) {
			LOG_DEBUG << "Scene already has one mesh.";
			return;
		}
		Mesh mesh;
		/// mesh loop
		for (std::vector<Mesh>::const_iterator m_it = mMeshes.begin();
			m_it != mMeshes.end(); ++m_it) {
			/// vertex loop
			for (std::size_t v = 0; v < m_it->vertexCount(); ++v) {
				mesh.insertVertex(m_it->vertexPositions()[v]);
				if (m_it->hasNormals()) {
					mesh.insertNormal(m_it->normals()[v]);
				}
				if (m_it->hasTexCoords()) {
					mesh.insertTexCoord(m_it->texCoords()[v]);
				}
				/// face loop
				for (std::size_t f = 0; f < m_it->faceCount(); ++f) {
					mesh.insertFaceIndices(m_it->faceIndices()[f]);
					mesh.insertMaterialIndex(m_it->materialIndices()[f]);
				}	//	!face loop
			}	//	!vertex loop
		}	//	!mesh loop
		mMeshes.clear();
		insertMesh(mesh);
	}

}	//	!namespace scene
#endif // !INCLUDE_SCENE_SCENE_H

