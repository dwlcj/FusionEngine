#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_ANY_HIT_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_ANY_HIT_PROGRAM_H
/// Fusion
#include <optix_rendering_system/rt_program.h>
#include <ptx_compiler/ptx_compiler.h>
/// STL
#include <memory>
namespace rt {
	class AnyHitProgram : public RTProgram {
		explicit AnyHitProgram() { }
		explicit AnyHitProgram(
			optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const std::shared_ptr<ptx::PTXCompiler> compiler);
	};

	/**
	*	Constructor
	*/
	AnyHitProgram::AnyHitProgram(
		optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const std::shared_ptr<ptx::PTXCompiler> compiler)
		: RTProgram(ctx, name, ptxPath, cuPath, compiler)
	{ }
}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_ANY_HIT_PROGRAM_H

