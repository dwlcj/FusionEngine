#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_SIMPLE_MISS_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_SIMPLE_MISS_PROGRAM_H
/// RX
#include <rxcpp/rx.hpp>
/// Fusion
#include <optix_rendering_system/float3_program.h>
#include <comm_system/float3_program_message.h>
#include <ptx_compiler/ptx_compiler.h>
/// STL
#include <functional>
#include <memory>
namespace rt {
	/**
	*	Represents Simple Miss Program (with 1 parameter that represents the 
	*	background color when a ray misses).
	*/
	class SimpleMissProgram : public Float3Program {
	public:
		explicit SimpleMissProgram() { }
		explicit SimpleMissProgram(optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const optix::float3& bgColor,
			const std::shared_ptr<ptx::PTXCompiler> compiler);
		void setMissColor(const optix::float3& color);
		std::function<void(const comm::Float3ProgramMessage&)> colorFlowIn();
	};

	/**
	*	Constructor
	*/
	SimpleMissProgram::SimpleMissProgram(optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const optix::float3& bgColor,
		const std::shared_ptr<ptx::PTXCompiler> compiler)
		: Float3Program(ctx, name, ptxPath, cuPath, bgColor, compiler)
	{ }

	/**
	*	Sets value
	*/
	void SimpleMissProgram::setMissColor(const optix::float3& color) {
		this->setValue(color);
		this->program()["bg_color"]->setFloat(color);
	}

	/**
	*	
	*/
	std::function<void(const comm::Float3ProgramMessage&)> SimpleMissProgram::colorFlowIn() {
		return [this](const comm::Float3ProgramMessage& message) {
			setMissColor(message.val());
		};
	}
}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_SIMPLE_MISS_PROGRAM_H

