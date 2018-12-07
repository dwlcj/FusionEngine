#ifndef INCUDE_OPTIX_RENDERING_SYSTEM_CLOSEST_HIT_PROGRAM
#define INCUDE_OPTIX_RENDERING_SYSTEM_CLOSEST_HIT_PROGRAM
/// Fusion
#include <optix_rendering_system/rt_program.h>
#include <ptx_compiler/ptx_compiler.h>
/// STL
#include <memory>
namespace rt {
	class ClosestHitProgram : public RTProgram {
	public:
		ClosestHitProgram() { }
		ClosestHitProgram(
			optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const std::shared_ptr<ptx::PTXCompiler> compiler);
	};

	/**
	*	Constructor
	*/
	ClosestHitProgram::ClosestHitProgram(
		optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const std::shared_ptr<ptx::PTXCompiler> compiler) 
		: RTProgram(ctx, name, ptxPath, cuPath, compiler)
	{ }

}	//	!namespace rt
#endif // !INCUDE_OPTIX_RENDERING_SYSTEM_CLOSEST_HIT_PROGRAM

