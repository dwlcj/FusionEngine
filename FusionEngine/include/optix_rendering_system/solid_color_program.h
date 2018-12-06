#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_SOLID_COLOR_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_SOLID_COLOR_PROGRAM_H
/// Fusion
#include <optix_rendering_system/float3_program.h>
#include <comm_system/float3_program_message.h>
#include <ptx_compiler/ptx_compiler.h>
/// STL
#include <memory>
#include <functional>
namespace rt {
	/**
	*	Represents the "empty" camera program (with 1 parameter, i.e. color to show on screen)
	*/
	class SolidColorProgram : public Float3Program {
	public:
		explicit SolidColorProgram() { }
		explicit SolidColorProgram(
			optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const optix::float3& bgColor,
			std::shared_ptr<ptx::PTXCompiler> compiler);
		void setBgColor(const optix::float3& color);
		std::function<void(const comm::Float3ProgramMessage&)> colorFlowIn();
	};

	/**
	*	Constructor
	*/
	SolidColorProgram::SolidColorProgram(
		optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const optix::float3& bgColor,
		std::shared_ptr<ptx::PTXCompiler> compiler)
		: Float3Program(ctx, name, ptxPath, cuPath, bgColor, compiler)
	{ 
		setBgColor(bgColor);
	}

	/**
	*	sets Backgound color
	*/
	void SolidColorProgram::setBgColor(const optix::float3& color) {
		this->setValue(color);
		this->program()["draw_color"]->setFloat(color);
	}

	/**
	*	Message Flow in
	*/
	std::function<void(const comm::Float3ProgramMessage&)> SolidColorProgram::colorFlowIn() {
		return [this](const comm::Float3ProgramMessage& message) {
			setBgColor(message.val());
		};
	}

	
}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_SOLID_COLOR_PROGRAM_H

