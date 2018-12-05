#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_RT_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_RT_PROGRAM_H
/// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_vector_types.h>
/// Logging
#include <plog/Log.h>
/// STL
#include <string>
namespace rt {
	/**
	*	Program Shared Data
	*/
	struct ProgramData {
		std::string name;
		std::string ptxPath;
		std::string cuPath;
		optix::Program program;
		optix::Context context;
	};

	/**
	*	Rt Program base class
	*/
	class RTProgram {
	public:
		RTProgram(
			optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath)
		{
			mData.context = ctx;
			mData.name = name;
			mData.ptxPath = ptxPath;
			mData.cuPath = cuPath;
			try {
				mData.program = mData.context->createProgramFromPTXFile(mData.ptxPath, mData.name);
			}
			catch (optix::Exception& ex) {
				LOG_ERROR << ex.getErrorCode() << ": " << ex.getErrorString();
			}
		}
		const std::string name() const { return mData.name; }
		const std::string ptxPath() const { return mData.ptxPath; }
		const std::string cuPath() const { return mData.cuPath; }
		optix::Program& program() { return mData.program; }
	private:
		ProgramData mData;
	};
}
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_RT_PROGRAM_H

