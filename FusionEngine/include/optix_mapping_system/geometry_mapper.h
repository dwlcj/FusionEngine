#ifndef _INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_MAPPER_H_
#define _INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_MAPPER_H_

// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_aabb_namespace.h>

// scene
#include <scene/mesh.h>

namespace map {
	class GeometryMapper {
	public:
		explicit GeometryMapper(
			optix::Context& ctx,
			const std::string& bboxProgCuFilepath,
			const std::string& interProgCuFilepath);
		explicit GeometryMapper(
			optix::Context& ctx,
			const optix::Program& bboxProg,
			const optix::Program& intersectionProg);
		void map(const scene::Mesh& mesh);
	private:
		optix::Program mBboxProgram;
		optix::Program mIntersectionProgram;
		
	};
}	//	!namespace map
#endif // !_INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_MAPPER_H_

