#ifndef _INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_MAPPER_H_
#define _INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_MAPPER_H_

#include <optix_mapping_system/mapper.h>
#include <scene/mesh.h>

#include <string>
namespace map {
	
	class GeometryMapper : public Mapper<scene::Mesh> {
	public:
		explicit GeometryMapper(optix::Context& ctx,
			const std::string& bboxProgCuFilepath,
			const std::string& interProgCuFilepath);
		explicit GeometryMapper(optix::Context& ctx,
			const optix::Program& bboxProg,
			const optix::Program& interProg);
		void map(const scene::Mesh& mesh) override;
	private:
		optix::Program mBoundingBoxProgram;
		optix::Program mIntersectionProgram;
	};
}	//	!namespace map
#endif // !_INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_MAPPER_H_

