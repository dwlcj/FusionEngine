#ifndef INCLUDE_COM_SYSTEM_FLOAT_PROGRAM_MESSAGE_H
#define INCLUDE_COM_SYSTEM_FLOAT_PROGRAM_MESSAGE_H
/// OptiX
#include <optixu/optixpp_namespace.h>
namespace comm {
	class Float3ProgramMessage {
	public:
		Float3ProgramMessage(const optix::float3& val)
			: mVal(val)
		{ }
		const optix::float3 val() const { return mVal; }
	private:
		optix::float3 mVal;
	};
}
#endif // !INCLUDE_COM_SYSTEM_FLOAT_PROGRAM_MESSAGE_H

