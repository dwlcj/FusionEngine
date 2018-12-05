#ifndef INCLUDE_COMM_SYSTEM_CAMERA_MESSAGE_H
#define INCLUDE_COMM_SYSTEM_CAMERA_MESSAGE_H
/// OptiX
#include <optixu/optixpp_namespace.h>
namespace comm {
	class CameraMessage {
	public:
		CameraMessage(
			const optix::float3& eye,
			const optix::float3& lookat,
			const optix::float3& up)
			: mEye(eye), mLookAt(lookat), mUp(up)
		{ }
		const optix::float3 eye() const { return mEye; }
		const optix::float3 lookat() const { return mLookAt; }
		const optix::float3 up() const { return mUp; }
	private:
		optix::float3 mEye;
		optix::float3 mLookAt;
		optix::float3 mUp;
	};
}	//	!namespace comm
#endif // !INCLUDE_COMM_SYSTEM_CAMERA_MESSAGE_H

