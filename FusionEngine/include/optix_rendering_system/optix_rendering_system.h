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
			const std::size_t& height);
		void render(const unsigned int& launchIndex, optix::Context& ctx);
		std::function<void(const comm::OptiXRendererMessage&)> messageFlowIn();
		std::function<void(const optix::Group&)> topObjectFlowIn();
	protected:
		void createPboBuffer(optix::Context& ctx);
	private:
		std::size_t mLaunchWidth;
		std::size_t mLaunchHeight;
		optix::Buffer mOutBuffer;
		GLuint mPBO;
		GLuint mTex;
	};

	/** Constructor
	*/
	OptiXRenderer::OptiXRenderer(
		optix::Context& ctx,
		const std::size_t& width,
		const std::size_t& height)
		: mLaunchWidth(width), 
		mLaunchHeight(height)
	{
		createPboBuffer(ctx);
	}



	/** Creates output buffer (GL interop)
	*/
	void OptiXRenderer::createPboBuffer(optix::Context& ctx) {
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
		mOutBuffer = ctx->createBufferFromGLBO(RT_BUFFER_OUTPUT, mPBO);
		mOutBuffer->setFormat(RT_FORMAT_UNSIGNED_BYTE4);
		mOutBuffer->setSize(mLaunchWidth, mLaunchHeight);
		ctx["output_buffer"]->setBuffer(mOutBuffer);
		ctx["scene_epsilon"]->setFloat(1.e-2f);
	}

	/** To be called in the game loop
	*	Renders the scene
	*/
	void OptiXRenderer::render(const unsigned int& launchIndex, optix::Context& ctx) {
		ctx->launch(launchIndex, mLaunchWidth, mLaunchHeight);
		
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
			
		};
	}


	/**
	*
	*/
	std::function<void(const optix::Group&)> OptiXRenderer::topObjectFlowIn() {
		return [this](const optix::Group& topObj) {
			// TODO: A lot to do here
			// Must find a way of beautifull communication
			// between programs
			// Maybe I should attach geometry related variables to geometry instance
			// groups to context
			//if (!mTopExists) {
			//	mContext["top_object"]->set(topObj);
			//	mContext["top_shadower"]->set(topObj);
			//	mPinholeCameraProgram->setTopObject(topObj);
			//	mContext->setEntryPointCount(2u);
			//	mContext->setRayGenerationProgram(1u, mPinholeCameraProgram->program());
			//}
		};
	}

}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_OPTIX_RENDERING_SYSTEM_H

