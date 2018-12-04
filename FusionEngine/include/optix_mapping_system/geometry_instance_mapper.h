#ifndef _INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_INSTANCE_MAPPER_H_
#define _INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_INSTANCE_MAPPER_H_

// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_aabb_namespace.h>

// scene
#include <optix_mapping_system/mapper.h>
#include <scene/material.h>

namespace map {
	class GinstanceMapper : public Mapper<optix::GeometryInstance, optix::Geometry, scene::Material> {
	public:
		explicit GinstanceMapper(
			optix::Context& ctx,
			const std::string& bboxProgCuFilepath,
			const std::string& interProgCuFilepath);
		explicit GinstanceMapper(
			optix::Context& ctx,
			const optix::Program& bboxProg,
			const optix::Program& intersectionProg);
		const optix::GeometryInstance map(const optix::Geometry& geometry, const scene::Material& mesh);

	private:
		optix::Program mBboxProgram;
		optix::Program mIntersectionProgram;

	};
}	//	!namespace map
#endif // !_INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_INSTANCE_MAPPER_H_
