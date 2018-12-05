#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_MISS_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_MISS_PROGRAM_H
/// RX
#include <rxcpp/rx.hpp>
/// Fusion
#include <optix_rendering_system/rt_program.h>
#include <comm_system/float3_program_message.h>
/// STL
#include <functional>
namespace rt {
	class MissProgram : public RTProgram {
	public:
		MissProgram(optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const optix::float3& bgColor);
		const optix::float3 bgColor() const { return mBgColor; }
		void setBGColor(const optix::float3& color);
		std::function<void(const comm::Float3ProgramMessage&)> colorFlowIn();
	private:
		optix::float3 mBgColor;
		
	};

	/**
	*	Constructor
	*/
	MissProgram::MissProgram(optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const optix::float3& bgColor)
		: RTProgram(ctx, name, ptxPath, cuPath), mBgColor(bgColor)
	{ }

	/**
	* sets miss color (bg)
	*/
	void MissProgram::setBGColor(const optix::float3& color) {
		mBgColor = color;
		this->program()["bg_color"]->setFloat(color);
	}

	/**
	*	
	*/
	std::function<void(const comm::Float3ProgramMessage&)> MissProgram::colorFlowIn() {
		return [this](const comm::Float3ProgramMessage& message) {
			this->setBGColor(message.val());
		};
	}
}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_MISS_PROGRAM_H

