#ifndef INCLUDE_COMM_SYSTEM_OPTIX_RENDERER_MESSAGE_H
#define INCLUDE_COMM_SYSTEM_OPTIX_RENDERER_MESSAGE_H
namespace comm {
	class OptiXRendererMessage {
	public:
		explicit OptiXRendererMessage(const bool& topExists);
		const bool topExists() const { return mTopExists; }
	private:
		bool mTopExists;
	};

	/**
	*	Constructor
	*/
	OptiXRendererMessage::OptiXRendererMessage(const bool& topExists)
		: mTopExists(topExists)
	{ }
}	//	!namespace comm
#endif // !INCLUDE_COMM_SYSTEM_OPTIX_RENDERER_MESSAGE_H

