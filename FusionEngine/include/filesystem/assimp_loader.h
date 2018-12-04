#ifndef INCLUDE_FILESYSTEM_ASSIMP_LOADER_H
#define INCLUDE_FILESYSTEM_ASSIMP_LOADER_H

#include <optixu/optixu_aabb_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
//
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
//
#include <plog/Log.h>
//
#include <rxcpp/rx.hpp>
//
#include <scene/scene.h>
//
#include <memory>
#include <filesystem>
namespace fs {
	/*! Loads 3D files using assimp (Open Asset Import Library)
	*/
	class AssimpLoader {
	public:
		explicit AssimpLoader();
		void scanFile(const aiScene* scene);
		const bool isSupported(const std::string& filepath);
		const aiScene* loadFile(const std::string& filepath);
		const scene::Mesh importMesh(const aiMesh* m);
		const scene::Material importMaterial(const aiMaterial* m);
		const scene::Scene importScene(const aiScene* scene);
	private:
		std::shared_ptr<Assimp::Importer> mAssimpImporter;
	};

	/*! Constructor - Initializes AssimpImporter
	*/
	AssimpLoader::AssimpLoader() {
		mAssimpImporter = std::make_shared<Assimp::Importer>();
	}

	/*! Scans a 3D file.
		Logs (Only when Configuration is Debug): 
		Numbert of meshes
		Number of vertices
		Number of faces
		Number of normals
		Number of texture coordinates
	*/
	void AssimpLoader::scanFile(const aiScene* scene) {
		LOG_DEBUG << "Scanning file...";
		std::size_t vertices, indices, normals, texcoords;
		vertices = indices = normals = texcoords = 0;
		if (scene != nullptr) {
			for (std::size_t mesh = 0; mesh < scene->mNumMeshes; ++mesh) {
				vertices += scene->mMeshes[mesh]->mNumVertices;
				indices += scene->mMeshes[mesh]->mNumFaces;
				if (scene->mMeshes[mesh]->HasNormals()) {
					normals += scene->mMeshes[mesh]->mNumVertices;
				}
				if (scene->mMeshes[mesh]->HasTextureCoords(0)) {
					texcoords += scene->mMeshes[mesh]->mNumVertices;
				}
			}
		}
		LOG_DEBUG << "Scanned File Attributes:";
		LOG_DEBUG << "\tNumber of Meshes: " << scene->mNumMeshes;
		LOG_DEBUG << "\tNumber of Vertices: " << vertices;
		LOG_DEBUG << "\tNumber of Faces (Triangles): " << indices;
		LOG_DEBUG << "\tNumber of Normals: " << normals;
		LOG_DEBUG << "\tNumber of Texture Coordinates: " << texcoords;
	}

	/*! Checks if a filetype is supported
	*/
	const bool AssimpLoader::isSupported(const std::string& filepath) {
		std::experimental::filesystem::path path = std::experimental::filesystem::path(filepath);
		return mAssimpImporter->IsExtensionSupported(path.extension().generic_string());
	}

	/*!	 Loads 3D graphics file using Assimp
	*/
	const aiScene* AssimpLoader::loadFile(const std::string& filepath) {
		// Check if filetype is supported
		if (!isSupported(filepath)) {
			LOG_ERROR << "File type is not supported.";
		}
		// Attempt to load file
		LOG_DEBUG << "Attempting to load file: " << filepath;
		const aiScene* scene = mAssimpImporter->ReadFile(filepath, aiProcess_ValidateDataStructure);
		if (!scene) {
			LOG_ERROR << "Error while reading file: " << filepath;
			LOG_ERROR << mAssimpImporter->GetErrorString();
		}
#if _DEBUG
		// scans file in debug configuration
		scanFile(scene);
#endif
		return scene;
	}

	/*! maps aiMesh to FusionEngine's Mesh format
	*/
	const scene::Mesh AssimpLoader::importMesh(const aiMesh* m) {
		scene::Mesh mesh;
		// gathers vertices, normals, texture coordinates
		for (std::size_t v = 0; v < m->mNumVertices; ++v) {
			// vertices
			optix::float3 vertex = optix::make_float3(
				m->mVertices[v].x,
				m->mVertices[v].y, 
				m->mVertices[v].z);
			mesh.insertVertex(vertex);

			// normals
			if (m->HasNormals()) {
				optix::float3 norm = optix::make_float3(
					m->mNormals[v].x,
					m->mNormals[v].y,
					m->mNormals[v].z);
				mesh.insertNormal(norm);
			}
			// texture coordinates
			if (m->HasTextureCoords(0)) {
				optix::float2 texcoord = optix::make_float2(
					m->mTextureCoords[0][v].x,
					m->mTextureCoords[0][v].y);
				mesh.insertTexCoord(texcoord);
			}
		}	//	!vertex loop
		for (std::size_t f = 0; f < m->mNumFaces; ++f) {
			optix::uint3 indices = optix::make_uint3(
				m->mFaces[f].mIndices[0],
				m->mFaces[f].mIndices[1],
				m->mFaces[f].mIndices[2]);
			mesh.insertFaceIndices(indices);
		}	//	!face loop
		// material index
		mesh.insertMaterialIndex(m->mMaterialIndex);
		return mesh;
	}

	/*! maps assimp material to FusionEngine's material type
	*/
	const scene::Material AssimpLoader::importMaterial(const aiMaterial* m) {
		std::string name;
		aiColor3D* diff;
		aiColor3D* spec;
		aiColor3D* rad;
		aiColor3D* amb;
		float shin;
		optix::float3 matDiff, matSpec, matRad, matAmb;
		float matShin;
		// get name
		if (m->Get(AI_MATKEY_NAME, name) != aiReturn_SUCCESS) {
			LOG_DEBUG << "Material does not have a name";
			name = std::string("noName");
		}
		// get diffuse color
		if (m->Get(AI_MATKEY_COLOR_DIFFUSE, diff) == aiReturn_SUCCESS) {
			matDiff = optix::make_float3(diff->r, diff->g, diff->b);
		}
		else {
			matDiff = optix::make_float3(0.5f, 0.5f, 0.5f);
		}
		// get specular color
		if (m->Get(AI_MATKEY_COLOR_SPECULAR, spec) == aiReturn_SUCCESS) {
			matSpec = optix::make_float3(spec->r, spec->g, spec->b);
		}
		else {
			matSpec = optix::make_float3(0.5f, 0.5f, 0.5f);
		}
		// get radiance color
		if (m->Get(AI_MATKEY_COLOR_REFLECTIVE, rad) == aiReturn_SUCCESS) {
			matRad = optix::make_float3(rad->r, rad->g, rad->b);
		}
		else {
			matRad = optix::make_float3(0.5f, 0.5f, 0.5f);
		}
		// get ambient color
		if (m->Get(AI_MATKEY_COLOR_AMBIENT, amb) == aiReturn_SUCCESS) {
			matAmb = optix::make_float3(amb->r, amb->g, amb->b);
		}
		else {
			matAmb = optix::make_float3(0.5f, 0.5f, 0.5f);
		}
		// get shininess
		if (m->Get(AI_MATKEY_SHININESS, shin) == aiReturn_SUCCESS) {
			matShin = shin;
		}
		else {
			matShin = 0.0f;
		}
		return scene::Material(name, matDiff, matSpec, matRad, matAmb, matShin);
	}

	/*! Maps Assimp Scene to FusionEngine's type
	*/
	const scene::Scene AssimpLoader::importScene(const aiScene* scene) {
		scene::Scene fusionScene;
		if (scene != nullptr) {
			// mesh loop
			for (std::size_t m = 0; m < scene->mNumMeshes; ++m) {
				fusionScene.insertMesh(importMesh(scene->mMeshes[m]));
			}
			// material loop
			for (std::size_t m = 0; m < scene->mNumMaterials; ++m) {
				fusionScene.insertMaterial(importMaterial(scene->mMaterials[m]));
			}
		}
		return fusionScene;
	}

}	// !namespace fs
#endif // !INCLUDE_FILESYSTEM_ASSIMP_LOADER_H

