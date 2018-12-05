#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_SOLID_COLOR_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_SOLID_COLOR_PROGRAM_H
/// Fusion
#include <optix_rendering_system/rt_program.h>
#include <comm_system/float3_program_message.h>
/// STL
#include <functional>
namespace rt {
	class SolidColorProgram : public RTProgram {
	public:
		SolidColorProgram(
			optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const optix::float3& bgColor = optix::make_float3(0.2f, 0.2f, 0.2f));
		const optix::float3 bgColor() const { return mBgColor; }
		std::function<void(const comm::Float3ProgramMessage&)> colorFlowIn();
		void setBgColor(const optix::float3& color);
	private:
		optix::float3 mBgColor;
	};

	SolidColorProgram::SolidColorProgram(
		optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const optix::float3& bgColor)
		: RTProgram(ctx, name, ptxPath, cuPath), mBgColor(bgColor)
	{ }

	std::function<void(const comm::Float3ProgramMessage&)> SolidColorProgram::colorFlowIn() {
		return [this](const comm::Float3ProgramMessage& message) {
			setBgColor(message.val());
		};
	}

	void SolidColorProgram::setBgColor(const optix::float3& color) {
		mBgColor = color;
		this->program()["draw_color"]->setFloat(mBgColor);
	}
}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_SOLID_COLOR_PROGRAM_H

