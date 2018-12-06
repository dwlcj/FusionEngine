#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_H
/// Fusion
#include <optix_rendering_system/rt_program.h>
#include <ptx_compiler/ptx_compiler.h>
/// STL
#include <memory>
namespace rt {
	class BoundingBoxProgram : public RTProgram {
	public:
		explicit BoundingBoxProgram() { }
		explicit BoundingBoxProgram(
			optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const std::shared_ptr<ptx::PTXCompiler> compiler);
	private:
		optix::Buffer mVertexBuffer;
		optix::Buffer mIndexBuffer;
	};

	/**
	*	Constructor
	*/
	BoundingBoxProgram::BoundingBoxProgram(
		optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const std::shared_ptr<ptx::PTXCompiler> compiler)
		: RTProgram(ctx, name, ptxPath, cuPath, compiler)
	{ }

}
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_H

