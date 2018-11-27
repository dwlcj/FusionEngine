#ifndef _INCLUDE_SCENE_MESH_H_
#define _INCLUDE_SCENE_MESH_H_

#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_aabb_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>

//
#include <vector>
#include <string>

namespace scene {
	/*! Represents Mesh in a 3D scene
		and holds Mesh-related attributes
	*/
	class Mesh {
	public:
		explicit Mesh();
		explicit Mesh(const std::size_t& id, const std::string& name);
		const std::vector<optix::float3> vertexPositions() const { return mVertexPositions; }
		const std::vector<optix::uint3> faceIndices() const { return mFaceIndices; }
		const std::vector<optix::float3> normals() const { return mNormals; }
		const std::vector<optix::float2> texCoords() const { return mTexCoords; }
		const std::vector<optix::uint> materialIndices() const { return mMaterialIndices; }
		const optix::Aabb bbox() const { return mBbox; }
		const bool hasNormals();
		const bool hasTexCoords();
		const std::size_t vertexCount() { return mVertexPositions.size(); }
		const std::size_t faceCount() { return mFaceIndices.size(); }
		const std::size_t normalsCount() { return mNormals.size(); }
		const std::size_t texCoordsCount() { return mTexCoords.size(); }
		const std::size_t materialsCount() { return mMaterialIndices.size(); }
		void insertVertex(const optix::float3& vertex);
		void insertFaceIndices(const optix::uint3& faceIndices);
		void insertNormal(const optix::float3& normal);
		void insertTexCoord(const optix::float2& texCoord);
		void insertMaterialIndex(const optix::uint& matIndex);
		void setBbox(const optix::float3& min, const optix::float3& max);
		void center();
	private:
		std::size_t mId;
		std::string mName;
		std::vector<optix::float3> mVertexPositions;
		std::vector<optix::uint3> mFaceIndices;
		std::vector<optix::float3> mNormals;
		std::vector<optix::float2> mTexCoords;
		std::vector<optix::uint> mMaterialIndices;
		optix::Aabb mBbox;
	};

	/*! Empty Constructor
		Initializes Bounding Box
	*/
	Mesh::Mesh() {
		mBbox.m_min.x = mBbox.m_min.y = mBbox.m_min.z = 1e16f;
		mBbox.m_max.x = mBbox.m_max.y = mBbox.m_max.z = -1e16f;
	}

	/*! Contructor with id and name
		Sets Mesh's id and name and initializes its Bounding Box
	*/
	Mesh::Mesh(const std::size_t& id, const std::string& name)
		: mId(id), mName(name)
	{
		mBbox.m_min.x = mBbox.m_min.y = mBbox.m_min.z = 1e16f;
		mBbox.m_max.x = mBbox.m_max.y = mBbox.m_max.z = -1e16f;
	}

	/*! Checks if normals vector is empty
		Returns true if not empty
	*/
	const bool Mesh::hasNormals() {
		bool tmp;
		tmp = (mNormals.empty()) ? false : true;
		return tmp;
	}

	/*! Checks if texture coordinates vector is empty
	Returns true if not empty
	*/
	const bool Mesh::hasTexCoords() {
		bool tmp;
		tmp = (mTexCoords.empty()) ? false : true;
		return tmp;
	}

	/*! Places vertex position in vertex vector
	*/
	void Mesh::insertVertex(const optix::float3& vertex) {
		mVertexPositions.emplace_back(vertex);
	}

	/*! Places face index in faces indices vector
	*/
	void Mesh::insertFaceIndices(const optix::uint3& faceIndices) {
		mFaceIndices.emplace_back(faceIndices);
	}

	/*! Places normal in normals vector
	*/
	void Mesh::insertNormal(const optix::float3& normal) {
		mNormals.emplace_back(normal);
	}

	/*! Places texture coordinate in texture coordinates vector
	*/
	void Mesh::insertTexCoord(const optix::float2& texCoord) {
		mTexCoords.emplace_back(texCoord);
	}

	/*! Places material index in material indices vector
	*/
	void Mesh::insertMaterialIndex(const optix::uint& matIndex) {
		mMaterialIndices.emplace_back(matIndex);
	}

	/*! Calculates bounding box
	*/
	void Mesh::setBbox(const optix::float3& min, const optix::float3& max) {
		mBbox = optix::Aabb(min, max);
	}

}	//	!namespace scene
#endif // !_INCLUDE_SCENE_MESH_H_

