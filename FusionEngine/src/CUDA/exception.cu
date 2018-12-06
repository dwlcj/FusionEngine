#include <optix_world.h>
#include <optixu/optixu_math_namespace.h>

using namespace optix;

rtBuffer<uchar4, 2>              output_buffer;
rtDeclareVariable(float3,        bad_color, , );
rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );

RT_PROGRAM void exception()
{
  const unsigned int code = rtGetExceptionCode();
  rtPrintf( "Caught exception 0x%X at launch index (%d,%d)\n", code, launch_index.x, launch_index.y );
  output_buffer[launch_index] = make_uchar4( bad_color.x * 255, bad_color.y * 255, bad_color.z * 255, 255);
}