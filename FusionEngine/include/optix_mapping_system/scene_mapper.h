#ifndef _INCLUDE_OPTIX_MAPPING_SYSTEM_SCENE_MAPPER_H_
#define _INCLUDE_OPTIX_MAPPING_SYSTEM_SCENE_MAPPER_H_

#include <optix_mapping_system/geometry_mapper.h>
#include <optix_mapping_system/geometry_instance_mapper.h>

#include <memory>

namespace map {
	class SceneMapper {
	public:
		explicit SceneMapper(optix::Context& ctx);
	private:
		optix::Group mGroupNode;
		std::vector<optix::Transform> mTransformNodes;
		std::shared_ptr<GeometryMapper> mGeometryMapper;
		std::shared_ptr<GinstanceMapper> mGinstanceMapper;
	};

	SceneMapper::SceneMapper(optix::Context& ctx) {
		mGroupNode = ctx->createGroup();
		mGeometryMapper = std::make_shared<GeometryMapper>();
		mGinstanceMapper = std::make_shared<GinstanceMapper>();
	}
}
#endif // !_INCLUDE_OPTIX_MAPPING_SYSTEM_SCENE_MAPPER_H_

