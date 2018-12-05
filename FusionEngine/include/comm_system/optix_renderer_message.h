#ifndef INCLUDE_COMM_SYSTEM_OPTIX_RENDERER_MESSAGE_H
#define INCLUDE_COMM_SYSTEM_OPTIX_RENDERER_MESSAGE_H
/// STL
#include <vector>
namespace comm {
	/**
	*	Available Camera types
	*/
	enum class CamType : std::size_t {
		None = 0,
		Pinhole,
		Panoramic
	};

	/**
	*
	*/
	class OptiXRendererMessage {
	public:
		OptiXRendererMessage(const CamType& type)
			: mCamType(type)
		{ }
		const CamType cameraType() const { return mCamType; }
	private:
		CamType mCamType;
	};
}	//	!namespace comm
#endif // !INCLUDE_COMM_SYSTEM_OPTIX_RENDERER_MESSAGE_H

