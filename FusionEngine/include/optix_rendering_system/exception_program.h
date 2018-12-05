#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_EXCEPTION_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_EXCEPTION_PROGRAM_H
/// Fusion
#include <optix_rendering_system/rt_program.h>
#include <comm_system/float3_program_message.h>
/// STL
#include <string>
namespace rt {
	class ExceptionProgram : public RTProgram {
	public:
		ExceptionProgram(optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const optix::float3& badColor);
		const optix::float3& badColor() const { return mBadColor; }
		void setBadColor(const optix::float3& color);
		std::function<void(const comm::Float3ProgramMessage&)> colorFlowIn();
	private:
		optix::float3 mBadColor;
	};

	/**
	*	Constructor
	*/
	ExceptionProgram::ExceptionProgram(optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const optix::float3& badColor)
		: RTProgram(ctx, name, ptxPath, cuPath), mBadColor(badColor)
	{ }

	/**
	*	Sets Exception Program's bad color
	*/
	void ExceptionProgram::setBadColor(const optix::float3& color) {
		mBadColor = color;
		this->program()["bad_color"]->setFloat(color);
	}

	/**
	*	
	*/
	std::function<void(const comm::Float3ProgramMessage&)> ExceptionProgram::colorFlowIn() {
		return [this](const comm::Float3ProgramMessage& message) {
			this->setBadColor(message.val());
		};
	}
}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_EXCEPTION_PROGRAM_H

