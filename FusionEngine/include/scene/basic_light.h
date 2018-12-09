#ifndef INCLUDE_SCENE_BASIC_LIGHT_H
#define INCLUDE_SCENE_BASIC_LIGHT_H
/// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
namespace scene {
	struct BasicLight {
		optix::float3 pos;
		optix::float3 color;
		int casts_shadow;
		int padding;
	};
}	//	!namespace scene
#endif // !INCLUDE_SCENE_BASIC_LIGHT_H
