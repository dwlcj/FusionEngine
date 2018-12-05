#ifndef INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_MAPPER_H
#define INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_MAPPER_H

// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_aabb_namespace.h>

// scene
#include <optix_mapping_system/mapper.h>
#include <scene/mesh.h>

namespace map {
	
	class GeometryMapper : public Mapper<optix::Geometry, scene::Mesh> {
	public:
		explicit GeometryMapper(optix::Context& ctx);
		explicit GeometryMapper(
			optix::Context& ctx,
			const std::string& bboxProgCuFilepath,
			const std::string& interProgCuFilepath,
			const std::string& bboxProgName,
			const std::string& interProgName);
		
		const std::string bboxProgPath() const { return mBoundingBoxProgPath; }
		const std::string interProgPath() const { return mIntersectionProgPath; }
		const std::string bboxProgName() const { return mBoundingBoxProgName; }
		const std::string interProgName() const { return mIntersectionProgName; }

		void setBoundingBoxProgPath(const std::string& path);
		void setIntersectionProgPath(const std::string& path);
		void setBoundingBoxProgName(const std::string& name);
		void setIntersectionProgName(const std::string& name);
		void setBoundingBoxProg(const std::string& filepath, const std::string& name);
		void setIntersectionProg(const std::string& filepath, const std::string& name);

		const optix::Geometry map(const scene::Mesh& mesh) override;
	protected:
		/// Structure with geometry's buffers
		struct GeometryBuffers {
			optix::Buffer vertexBuffer;
			optix::Buffer indexBuffer;
			optix::Buffer normalsBuffer;
			optix::Buffer texcoordsBuffer;
			optix::Buffer materialBuffer;
		};
		/// Structure with geometry's programs
		struct GeometryPrograms {
			optix::Program boundingBoxProgram;
			optix::Program intersectionProgram;
		};
		GeometryBuffers setupBuffers(const scene::Mesh& mesh);
		void mapBuffers(const scene::Mesh& mesh, GeometryBuffers& buffers);
		GeometryPrograms generatePrograms();
		void addBuffersToPrograms(GeometryBuffers& buffers, GeometryPrograms& programs);
		void unmapBuffers(GeometryBuffers& buffers);
	private:
		optix::Context mContext;
		std::string mBoundingBoxProgPath;
		std::string mIntersectionProgPath;
		std::string mBoundingBoxProgName;
		std::string mIntersectionProgName;
	};

	/**
	*	Constructor from optix::Context
	*/
	GeometryMapper::GeometryMapper(optix::Context& ctx)
		: mContext(ctx)
	{ 
		// TODO: change these hardcoded abs paths and make relative
		// or maybe move the ptx files to the resources folder??
		// I dunno
		mBoundingBoxProgPath = "D:\\_dev\\_Projects\\_Visual_Studio\\FusionEngine\\FusionEngine\\src\\optix_mapping_system\\CUDA\\triangle_mesh.ptx";
		mBoundingBoxProgName = "mesh_bounds";
		mIntersectionProgPath = "D:\\_dev\\_Projects\\_Visual_Studio\\FusionEngine\\FusionEngine\\src\\optix_mapping_system\\CUDA\\triangle_mesh.ptx";
		mIntersectionProgName = "mesh_intersect";
	}

	/**
	*
	*/
	GeometryMapper::GeometryMapper(
		optix::Context& ctx, 
		const std::string& bboxProgFilepath, 
		const std::string& interProgFilepath,
		const std::string& bboxProgName,
		const std::string& interProgName) 
		: mContext(ctx), 
		mBoundingBoxProgPath(bboxProgFilepath),
		mIntersectionProgPath(interProgFilepath),
		mBoundingBoxProgName(bboxProgName),
		mIntersectionProgName(interProgName)
	{ }

	/**
	*	sets bounding box programs filepath
	*/
	void GeometryMapper::setBoundingBoxProgPath(const std::string& path) {
		mBoundingBoxProgPath = path;
	}

	/**
	*	sets bounding box program's name
	*/
	void GeometryMapper::setBoundingBoxProgName(const std::string& name) {
		mBoundingBoxProgName = name;
	}

	/**
	*	sets bounding box program attributes
	*/
	void GeometryMapper::setBoundingBoxProg(const std::string& filepath, const std::string& name) {
		mBoundingBoxProgPath = filepath;
		mBoundingBoxProgName = name;
	}

	/**
	*	sets intersection program's filepath
	*/
	void GeometryMapper::setIntersectionProgPath(const std::string& path) {
		mIntersectionProgPath = path;
	}

	/**
	*	sets intersections program's name
	*/
	void GeometryMapper::setIntersectionProgName(const std::string& name) {
		mIntersectionProgName = name;
	}

	/**
	*	sets intersection program's attributes
	*/
	void GeometryMapper::setIntersectionProg(const std::string& filepath, const std::string& name) {
		mIntersectionProgPath = filepath;
		mIntersectionProgName = name;
	}

	/**
	* Creates Geometry's buffers
	*/
	GeometryMapper::GeometryBuffers GeometryMapper::setupBuffers(const scene::Mesh& mesh) {
		GeometryMapper::GeometryBuffers buffers;
		buffers.vertexBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT3, mesh.vertexCount());
		buffers.indexBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_INT3, mesh.faceCount());
		buffers.normalsBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT3, mesh.normalsCount());
		buffers.texcoordsBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT2, mesh.texCoordsCount());
		buffers.materialBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_INT, mesh.faceCount());
		return buffers;
	}

	/**
	*	Maps mesh's data to OptiX buffers
	*/
	void GeometryMapper::mapBuffers(const scene::Mesh& mesh, GeometryBuffers& buffers) {
		// vertex data
		std::memcpy(reinterpret_cast<float*>(buffers.vertexBuffer->map()),
			mesh.vertexPositions().data(), mesh.vertexCount() * sizeof(optix::float3));
		// face index data
		std::memcpy(reinterpret_cast<int*>(buffers.indexBuffer->map()),
			mesh.faceIndices().data(), mesh.faceCount() * sizeof(optix::uint3));
		// normals data
		std::memcpy(reinterpret_cast<float*>(buffers.normalsBuffer->map()),
			mesh.normals().data(), mesh.normalsCount() * sizeof(optix::float3));
		// texcoords data
		std::memcpy(reinterpret_cast<float*>(buffers.texcoordsBuffer->map()),
			mesh.texCoords().data(), mesh.texCoordsCount() * sizeof(optix::float2));
		// materials data
		std::memcpy(reinterpret_cast<int*>(buffers.materialBuffer->map()),
			mesh.materialIndices().data(), mesh.faceCount() * sizeof(optix::uint));
	}

	/**
	*	generate's geometry's programs
	*/
	GeometryMapper::GeometryPrograms GeometryMapper::generatePrograms() {
		GeometryPrograms programs;
		programs.boundingBoxProgram = mContext->createProgramFromPTXFile(mBoundingBoxProgPath, mBoundingBoxProgName);
		programs.intersectionProgram = mContext->createProgramFromPTXFile(mIntersectionProgPath, mIntersectionProgName);
		return programs;
	}

	void GeometryMapper::addBuffersToPrograms(GeometryMapper::GeometryBuffers& buffers, GeometryMapper::GeometryPrograms& programs) {
		// bounding box program needs access to vertex buffer and index buffer
		programs.boundingBoxProgram["vertex_buffer"]->setBuffer(buffers.vertexBuffer);
		programs.boundingBoxProgram["index_buffer"]->setBuffer(buffers.indexBuffer);
		// intersection program needs acces to all of the geometry's buffers
		programs.intersectionProgram["vertex_buffer"]->setBuffer(buffers.vertexBuffer);
		programs.intersectionProgram["index_buffer"]->setBuffer(buffers.indexBuffer);
		programs.intersectionProgram["normal_buffer"]->setBuffer(buffers.normalsBuffer);
		programs.intersectionProgram["texcoord_buffer"]->setBuffer(buffers.texcoordsBuffer);
		programs.intersectionProgram["material_buffer"]->setBuffer(buffers.materialBuffer);
	}

	void GeometryMapper::unmapBuffers(GeometryMapper::GeometryBuffers& buffers) {
		buffers.vertexBuffer->unmap();
		buffers.indexBuffer->unmap();
		buffers.normalsBuffer->unmap();
		buffers.texcoordsBuffer->unmap();
		buffers.materialBuffer->unmap();
	}

	/**
		Maps scene::Mesh to optix::Geometry
	*/
	const optix::Geometry GeometryMapper::map(const scene::Mesh& mesh) {
		optix::Geometry geom = mContext->createGeometry();
		GeometryBuffers buffers = setupBuffers(mesh);
		mapBuffers(mesh, buffers);
		GeometryPrograms programs = generatePrograms();
		addBuffersToPrograms(buffers, programs);
		// assign programs
		geom->setBoundingBoxProgram(programs.boundingBoxProgram);
		geom->setIntersectionProgram(programs.intersectionProgram);
		return geom;
	}
}	//	!namespace map
#endif // !INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_MAPPER_H

