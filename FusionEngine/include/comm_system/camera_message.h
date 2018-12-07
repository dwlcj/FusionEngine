#ifndef INCLUDE_COMM_SYSTEM_CAMERA_MESSAGE_H
#define INCLUDE_COMM_SYSTEM_CAMERA_MESSAGE_H
/// OptiX
#include <optixu/optixpp_namespace.h>
/// STL
#include <vector>
namespace comm {
	/**
	*	Message Type Enum
	*	Technically specifies which value is modified
	*/
	enum class CamMessageType : std::size_t {
		None = 0,
		Eye,
		Lookat,
		Up,
		topExists
	};

	class CameraMessage {
	public:
		CameraMessage(
			const CamMessageType& type,
			const bool& topExists = false,
			const optix::Group& topObj = NULL,
			const optix::float3& eye = optix::make_float3(0.0f, 0.0f, 0.0f),
			const optix::float3& lookat = optix::make_float3(0.0f, 0.0f, 0.0f),
			const optix::float3& up = optix::make_float3(0.0f, 0.0f, 0.0f))
			: mEye(eye), mLookAt(lookat), mUp(up), mTopObject(topObj)
		{ }
		const CamMessageType type() const { return mType; }
		const optix::float3 eye() const { return mEye; }
		const optix::float3 lookat() const { return mLookAt; }
		const optix::float3 up() const { return mUp; }
		const bool& topExists() const { return mTopExists; }
		const optix::Group topObject() const { return mTopObject; }
	private:
		CamMessageType mType;
		optix::float3 mEye;
		optix::float3 mLookAt;
		optix::float3 mUp;
		bool mTopExists;
		optix::Group mTopObject;
	};
}	//	!namespace comm
#endif // !INCLUDE_COMM_SYSTEM_CAMERA_MESSAGE_H

