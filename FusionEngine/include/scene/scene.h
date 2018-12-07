#ifndef INCLUDE_SCENE_SCENE_H
#define INCLUDE_SCENE_SCENE_H

#include <optixu/optixu_aabb_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
//
#include <scene/mesh.h>
#include <scene/material.h>
//
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
	protected:
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
	{ }

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
}	//	!namespace scene
#endif // !INCLUDE_SCENE_SCENE_H

