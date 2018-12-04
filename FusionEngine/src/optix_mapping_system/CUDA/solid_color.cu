#include <optix.h>
#include <optixu/optixu_math_namespace.h>

rtDeclareVariable(optix::uint2, launch_index, rtLaunchIndex, );
rtBuffer<optix::float3, 2> output_buffer;

rtDeclareVariable(optix::float3, color_to_draw, , );

RT_PROGRAM void draw_solid_color()
{
	output_buffer[launch_index] = color_to_draw;
}