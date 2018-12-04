#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_OPTIX_RENDERING_SYSTEM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_OPTIX_RENDERING_SYSTEM_H

// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>

// GL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <mmsystem.h>

// logging
#include <plog/Log.h>

// stl
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
			const unsigned int& launchIndex = 0u);
		void render(const unsigned int& launchIndex);
	protected:
		void createPboBuffer();
	private:
		optix::Context& mContext;
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
		const std::size_t& height, 
		const unsigned int& launchIndex) 
		: mContext(ctx), mLaunchWidth(width), mLaunchHeight(height)
	{ 
		createPboBuffer();
	}

	/** Creates output buffer (GL interop)
	*/
	void OptiXRenderer::createPboBuffer() {
		glGenBuffers(1, &mPBO);
		if (mPBO == 0) {
			LOG_ERROR << "OptiX Renderer Error (OpenGL): Cannot generate PBO.";
		}
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPBO);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, mLaunchHeight * mLaunchHeight * sizeof(unsigned char) * 4, nullptr, GL_STREAM_READ);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

		glGenTextures(1, &mTex);
		if (mTex == 0) {
			LOG_ERROR << "OptiX Renderer Error (OpenGL): Cannot generate Texture.";
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
	void OptiXRenderer::render(const unsigned int& launchIndex) {
		mContext->launch(launchIndex, mLaunchWidth, mLaunchHeight);
		// Query buffer info
		RTsize bufferWRts, bufferHRts;
		mOutBuffer->getSize(bufferWRts, bufferHRts);
		const std::uint32_t width = static_cast<int>(bufferWRts);
		const std::uint32_t height = static_cast<int>(bufferHRts);
		const RTformat bufferFormat = mOutBuffer->getFormat();

		const GLvoid* imageData = mOutBuffer->map(0, RT_BUFFER_MAP_READ);
		GLenum glDataType = GL_FALSE;
		GLenum glFormat = GL_FALSE;

		switch (bufferFormat) {
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
		if (elementSize % 8 == 0)		align = 8;
		else if (elementSize % 4 == 0)	align = 4;
		else if (elementSize % 2 == 0)	align = 2;
		glPixelStorei(GL_UNPACK_ALIGNMENT, align);
		glDrawPixels(
			static_cast<GLsizei>(width),
			static_cast<GLsizei>(height),
			glFormat,
			glDataType,
			imageData);
		//glReadPixels(0, 0, mLaunchWidth, mLaunchHeight, GL_RGB, GL_UNSIGNED_BYTE, )
		mOutBuffer->unmap();
	}


}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_OPTIX_RENDERING_SYSTEM_H

