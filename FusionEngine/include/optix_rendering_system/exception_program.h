#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_EXCEPTION_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_EXCEPTION_PROGRAM_H
/// Fusion
#include <optix_rendering_system/float3_program.h>
#include <comm_system/float3_program_message.h>
/// STL
#include <string>
#include <functional>
#include <memory>
namespace rt {
	/**
	*	Represents Exception program (with 1 parameter that is the pixel
	*	color when exception is thrown for that pixel)
	*/
	class ExceptionProgram : public Float3Program {
	public:
		explicit ExceptionProgram() { }
		explicit ExceptionProgram(optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const optix::float3& badColor,
			const std::shared_ptr<ptx::PTXCompiler> compiler);
		void setBadColor(const optix::float3& color);
		std::function<void(const comm::Float3ProgramMessage&)> colorFlowIn();
	};

	/**
	*	Constructor
	*/
	ExceptionProgram::ExceptionProgram(optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const optix::float3& badColor,
		const std::shared_ptr<ptx::PTXCompiler> compiler)
		: Float3Program(ctx, name, ptxPath, cuPath, badColor, compiler)
	{ }

	/**
	*	Sets Exception Program's bad color
	*/
	void ExceptionProgram::setBadColor(const optix::float3& color) {
		this->setValue(color);
		this->program()["bad_color"]->setFloat(color);
	}

	/**
	*	Message Flow in
	*/
	std::function<void(const comm::Float3ProgramMessage&)> ExceptionProgram::colorFlowIn() {
		return [this](const comm::Float3ProgramMessage& message) {
			this->setBadColor(message.val());
		};
	}
}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_EXCEPTION_PROGRAM_H

