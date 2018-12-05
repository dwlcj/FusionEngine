#include <optix_world.h>

using namespace optix;

rtBuffer<uchar4, 2>              output_buffer;
rtDeclareVariable(float3,        bad_color, , );
rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );

RT_PROGRAM void exception()
{
  const unsigned int code = rtGetExceptionCode();
  rtPrintf( "Caught exception 0x%X at launch index (%d,%d)\n", code, launch_index.x, launch_index.y );
  output_buffer[launch_index] = make_color( bad_color );
}