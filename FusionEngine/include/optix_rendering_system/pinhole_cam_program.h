#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_PINHOLE_CAM_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_PINHOLE_CAM_PROGRAM_H
/// Fusion
#include <optix_rendering_system/rt_program.h>
#include <comm_system/camera_message.h>
/// STL
#include <functional>
namespace rt {
	class PinholeCamProgram : public RTProgram {
	public:
		PinholeCamProgram(optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const optix::float3& eye,
			const optix::float3& lookat,
			const optix::float3& up);
		const optix::float3 eye() const { return mEye; }
		const optix::float3 lookat() const { return mLookAt; }
		const optix::float3 up() const { return mUp; }
		std::function<void(const comm::CameraMessage&)> messageFlowIn();
	private:
		optix::float3 mEye;
		optix::float3 mLookAt;
		optix::float3 mUp;
	};
	/**
	*	Constructor
	*/
	PinholeCamProgram::PinholeCamProgram(
		optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const optix::float3& eye,
		const optix::float3& lookat,
		const optix::float3& up)
		: RTProgram(ctx, name, ptxPath, cuPath), mEye(eye), mLookAt(lookat), mUp(up)
	{ }

	/**
	*	Message Flow in
	*/
	std::function<void(const comm::CameraMessage&)> PinholeCamProgram::messageFlowIn() {
		return [this](const comm::CameraMessage& message) {
			mEye = message.eye();
			mLookAt = message.lookat();
			mUp = message.up();
			// TODO: change variables in device
		};
	}
}
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_PINHOLE_CAM_PROGRAM_H

