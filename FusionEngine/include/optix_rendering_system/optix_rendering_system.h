#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_OPTIX_RENDERING_SYSTEM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_OPTIX_RENDERING_SYSTEM_H
/// OptiX
#include <optixu/optixu_matrix_namespace.h>
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
/// GL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <mmsystem.h>
/// Logging
#include <plog/Log.h>
///Fusion
#include <optix_rendering_system/solid_color_program.h>
#include <optix_rendering_system/pinhole_cam_program.h>
#include <optix_rendering_system/simple_miss_program.h>
#include <optix_rendering_system/exception_program.h>
#include <ptx_compiler/ptx_compiler.h>
/// Communication
#include <comm_system/optix_renderer_message.h>
/// STL
#include <vector>
#include <string>
#include <functional>

namespace rt {
	// Renders scene
	class OptiXRenderer {
	public:
		explicit OptiXRenderer(
			optix::Context& ctx,
			const std::size_t& width,
			const std::size_t& height,
			std::shared_ptr<ptx::PTXCompiler> compiler);
		void render();
		std::function<void(const comm::OptiXRendererMessage&)> messageFlowIn();
	protected:
		void createPboBuffer();
		void setTopExists(const bool& exists);
	private:
		std::size_t mLaunchWidth;
		std::size_t mLaunchHeight;
		optix::Buffer mOutBuffer;
		GLuint mPBO;
		GLuint mTex;
		unsigned int mLaunchIndex;
		optix::Context& mContext;
		// wtf to do with programs
		std::shared_ptr<SolidColorProgram> mSolidColorProgram;
		std::shared_ptr<SimpleMissProgram> mSimpleMissProgram;
		std::shared_ptr<ExceptionProgram> mExceptionProgram;
		std::shared_ptr<PinholeCameraProgram> mPinholeCameraProgram;
		bool mTopExists;
	};

	/** Constructor
	*/
	OptiXRenderer::OptiXRenderer(
		optix::Context& ctx,
		const std::size_t& width,
		const std::size_t& height,
		std::shared_ptr<ptx::PTXCompiler> compiler)
		: mContext(ctx), 
		mLaunchWidth(width), 
		mLaunchHeight(height)
	{
		mTopExists = false;
		createPboBuffer();
		// Create Programs
		/// Solid Color Program
		mSolidColorProgram = std::make_shared<SolidColorProgram>(
			mContext,
			std::string("draw_solid_color"),
			std::string("\\res\\ptx\\draw_color.ptx"),
			std::string("..\\src\\CUDA\\draw_color.cu"),
			optix::make_float3(0.2f, 0.2f, 0.2f),
			compiler);
		/// Simple Miss Program
		mSimpleMissProgram = std::make_shared<SimpleMissProgram>(
			mContext,
			std::string("miss"),
			std::string("\\res\\ptx\\simple_miss.ptx"),
			std::string("..\\src\\CUDA\\constantbg.cu"),
			optix::make_float3(0.0f, 0.8f, 0.8f),
			compiler);
		/// Exception Program
		mExceptionProgram = std::make_shared<ExceptionProgram>(
			mContext,
			std::string("exception"),
			std::string("\\res\\ptx\\exception.ptx"),
			std::string("..\\src\\CUDA\\exception.cu"),
			optix::make_float3(1.0f, 0.0f, 0.0f),
			compiler);
		/// Pinhole Camera Program
		mPinholeCameraProgram =
			std::make_shared<PinholeCameraProgram>(
				mContext,
				std::string("pinhole_camera"),
				std::string("\\res\\ptx\\pinhole_camera.ptx"),
				std::string("..\\src\\CUDA\\pinhole_camera.cu"),
				compiler,
				mLaunchWidth, mLaunchHeight);
		// context related
		// TODO: add all entry points
		try {
			mContext->setEntryPointCount(2u);
			mContext->setRayTypeCount(1u);
			mContext->setRayGenerationProgram(0u, mSolidColorProgram->program());
			mContext->setExceptionProgram(0u, mExceptionProgram->program());
			mContext->setMissProgram(0u, mSimpleMissProgram->program());
			mContext->setRayGenerationProgram(1u, mPinholeCameraProgram->program());
			mContext->setExceptionProgram(1u, mExceptionProgram->program());
			mContext->setMissProgram(0u, mSimpleMissProgram->program());
		}
		catch (optix::Exception& ex) {
			LOG_ERROR << ex.getErrorCode() << ": " << ex.getErrorString();
		}
		
		mLaunchIndex = 0u;
		//mContext->setExceptionProgram(0u, mExceptionProgram->program());
		//mContext->setMissProgram(0u, mMissProgram->program());
	}



	/** Creates output buffer (GL interop)
	*/
	void OptiXRenderer::createPboBuffer() {
		glGenBuffers(1, &mPBO);
		if (mPBO == 0) {
			LOG_ERROR << "GL ERROR: PBO error";
		}
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPBO);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, mLaunchWidth * mLaunchHeight* sizeof(unsigned char) * 4, nullptr, GL_STREAM_READ);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

		glGenTextures(1, &mTex);
		if (mTex == 0) {
			LOG_ERROR << "GL ERROR: Texture generation error";
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		mOutBuffer = mContext->createBufferFromGLBO(RT_BUFFER_OUTPUT, mPBO);
		mOutBuffer->setFormat(RT_FORMAT_UNSIGNED_BYTE4);
		mOutBuffer->setSize(mLaunchWidth, mLaunchHeight);
		mContext["output_buffer"]->setBuffer(mOutBuffer);
		mContext["scene_epsilon"]->setFloat(1.e-3f);
	}

	/** To be called in the game loop
	*	Renders the scene
	*/
	void OptiXRenderer::render() {
		mContext->launch(mLaunchIndex, mLaunchWidth, mLaunchHeight);
		
		// Query buffer info
		RTsize bufferWRts, bufferHRts;
		mOutBuffer->getSize(bufferWRts, bufferHRts);
		const std::uint32_t width = static_cast<int>(bufferWRts);
		const std::uint32_t height = static_cast<int>(bufferHRts);
		const RTformat bufferFormat = mOutBuffer->getFormat();

		const GLvoid* imageData = mOutBuffer->map(0, RT_BUFFER_MAP_READ);
		GLenum glDataType = GL_FALSE;
		GLenum glFormat = GL_FALSE;

		switch (bufferFormat) 
		{
		case RT_FORMAT_UNSIGNED_BYTE4:
			glDataType = GL_UNSIGNED_BYTE;
			glFormat = GL_BGRA;
			break;
		case RT_FORMAT_FLOAT:
			glDataType = GL_FLOAT;
			glFormat = GL_LUMINANCE;
			break;
		case RT_FORMAT_FLOAT3:
			glDataType = GL_FLOAT;
			glFormat = GL_RGB;
			break;
		case RT_FORMAT_FLOAT4:
			glDataType = GL_FLOAT;
			glFormat = GL_RGBA;
			break;
		default:
			LOG_ERROR << "OptiX Renderer: Unrecognized buffer format.";
			break;
		}

		RTsize elementSize = mOutBuffer->getElementSize();
		int align = 1;
		if ((elementSize % 8) == 0)			align = 8;
		else if ((elementSize % 4) == 0)	align = 4;
		else if ((elementSize % 2) == 0)	align = 2;
		glPixelStorei(GL_UNPACK_ALIGNMENT, align);
		glDrawPixels(
			static_cast<GLsizei>(width),
			static_cast<GLsizei>(height),
			glFormat,
			glDataType,
			imageData);
		mOutBuffer->unmap();
	}

	/**
	*	Message Flow in
	*/
	std::function<void(const comm::OptiXRendererMessage&)> OptiXRenderer::messageFlowIn() {
		return [this](const comm::OptiXRendererMessage& message) {
			setTopExists(message.topExists());
		};
	}


}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_OPTIX_RENDERING_SYSTEM_H

