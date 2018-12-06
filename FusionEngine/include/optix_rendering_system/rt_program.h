#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_RT_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_RT_PROGRAM_H
/// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_vector_types.h>
/// Fusion
#include <ptx_compiler/ptx_compiler.h>
/// Logging
#include <plog/Log.h>
/// STL
#include <string>
#include <memory>
namespace rt {
	/**
	*	Program Shared Data
	*/
	struct ProgramData {
		std::string name;
		std::string ptxPath;
		std::string cuPath;
		std::string ptx;
		optix::Program program;
		optix::Context context;
	};

	/**
	*	Rt Program base class
	*/
	class RTProgram {
	public:
		explicit RTProgram() { }
		explicit RTProgram(
			optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const std::shared_ptr<ptx::PTXCompiler> compiler)
		{
			mData.context = ctx;
			mData.name = name;
			mData.ptxPath = ptxPath;
			mData.cuPath = cuPath;
#ifdef DEBUG
			try {
				std::string ptx;
				ptx = compiler->compileStr(mData.cuPath);
				mData.program = mData.context->createProgramFromPTXString(ptx, mData.name);
			}
			catch (optix::Exception& ex) {
				LOG_ERROR << ex.getErrorCode() << ": " << ex.getErrorString();
			}
#else
			try {
				mData.program = mData.context->createProgramFromPTXFile(mData.ptxPath, mData.name);
			}
			catch (optix::Exception& ex) {
				LOG_ERROR << ex.getErrorCode() << ": " << ex.getErrorString();
			}
#endif // DEBUG
		}		
		const std::string name() const { return mData.name; }
		const std::string ptxPath() const { return mData.ptxPath; }
		const std::string cuPath() const { return mData.cuPath; }
		void compile(std::shared_ptr<ptx::PTXCompiler> compiler) {
			mData.ptx = compiler->compileStr(mData.cuPath);
			mData.program = mData.context->createProgramFromPTXString(mData.ptx, mData.name);
		}
		optix::Program& program() { return mData.program; }
	private:
		ProgramData mData;
	};
}
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_RT_PROGRAM_H

