#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_INTERSECTION_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_INTERSECTION_PROGRAM_H
/// Fusion
#include <optix_rendering_system/rt_program.h>
#include <ptx_compiler/ptx_compiler.h>
/// STL
#include <memory>
namespace rt {
	/**
	*
	*/
	class IntersectionProgram : public RTProgram {
	public:
		explicit IntersectionProgram() { }
		explicit IntersectionProgram(
			optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const std::shared_ptr<ptx::PTXCompiler> compiler);
	};

	/**
	*
	*/
	IntersectionProgram::IntersectionProgram(
		optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const std::shared_ptr<ptx::PTXCompiler> compiler)
		: RTProgram(ctx, name, ptxPath, cuPath, compiler)
	{ }

}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_INTERSECTION_PROGRAM_H

