#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_PINHOLE_CAM_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_PINHOLE_CAM_PROGRAM_H
/// OptiX
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
/// Fusion
#include <optix_rendering_system/rt_program.h>
#include <comm_system/camera_message.h>
/// STL
#include <memory>
#include <functional>
namespace rt {
	/**
	*	Represents Pinhole Camera Program
	*/
	class PinholeCameraProgram : public RTProgram {
	public:
		explicit PinholeCameraProgram() { }
		explicit PinholeCameraProgram(
			optix::Context& ctx,
			const std::string& name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const std::shared_ptr<ptx::PTXCompiler> compiler,
			const std::size_t& viewportWidth,
			const std::size_t& viewportHeight);
		const optix::float3 eye() const { return mEye; }
		const optix::float3 lookat() const { return mLookat; }
		const optix::float3 up() const { return mUp; }
		void setEye(const optix::float3& eye);
		void setLookat(const optix::float3& lookat);
		void setUp(const optix::float3& up);
		std::function<void(const comm::CameraMessage&)> messageFlowIn();
	protected:
		void calculateCamParams();
		void updateParams();
	private:
		optix::float3 mEye;
		optix::float3 mLookat;
		optix::float3 mUp;
		optix::float3 mU;
		optix::float3 mV;
		optix::float3 mW;
		optix::Matrix4x4 mTransMat;
		float mFOV;
		float mAspectRatio;
		std::size_t mViewportWidth;
		std::size_t mViewportHeight;
	};

	/**
	*	Constructor
	*/
	PinholeCameraProgram::PinholeCameraProgram(
		optix::Context& ctx,
		const std::string& name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const std::shared_ptr<ptx::PTXCompiler> compiler,
		const std::size_t& viewportWidth,
		const std::size_t& viewportHeight)
		: RTProgram(ctx, name, ptxPath, cuPath, compiler), mViewportWidth(viewportWidth), mViewportHeight(viewportHeight)
	{
		mEye = optix::make_float3(0.0f, 0.0f, 0.0f);
		mLookat = optix::make_float3(0.0f, 0.0f, 1.0f);
		mUp = optix::make_float3(0.0f, -1.0f, 0.0f);
		mFOV = 35.0f;
		mAspectRatio = static_cast<float>(mViewportWidth) / static_cast<float>(mViewportHeight);
		mTransMat = optix::Matrix4x4::identity();
	}

	/**
	*	Sets Camera's eye (Programs eye variable)
	*/
	void PinholeCameraProgram::setEye(const optix::float3& eye) {
		mEye = eye;
		this->program()["eye"]->setFloat(mEye);
	}

	/**
	*	Sets Camera's lookat vector (Program's lookat variable)
	*/
	void PinholeCameraProgram::setLookat(const optix::float3& lookat) {
		mLookat = lookat;
		this->program()["lookat"]->setFloat(mLookat);
	}

	/**
	*	Sets Camera's up vector (Program's up variable)
	*/
	void PinholeCameraProgram::setUp(const optix::float3& up) {
		mUp = up;
		this->program()["up"]->setFloat(mUp);
	}

	/**
	*	Message Flow in
	*/
	std::function<void(const comm::CameraMessage&)> PinholeCameraProgram::messageFlowIn() {
		return [this](const comm::CameraMessage& message) {

		};
	}

	/**
	*	Calculates Camera space coordinates (U, V, W)
	*/
	void PinholeCameraProgram::calculateCamParams() {
		float ulen, vlen, wlen;
		mW = mLookat - mEye;
		wlen = optix::length(mW);
		mU = optix::normalize(optix::cross(mW, mUp));
		mV = optix::normalize(optix::cross(mU, mW));
		vlen = wlen * tanf(0.5f * mFOV * M_PIf / 180.0f);
		mV *= vlen;
		ulen = vlen * mAspectRatio;
		mU *= ulen;
	}

	/**
	*	Updates Camera's Parameters
	*/
	void PinholeCameraProgram::updateParams() {
		calculateCamParams();
		const optix::Matrix4x4 frame =
			optix::Matrix4x4::fromBasis(
				optix::normalize(mU), 
				optix::normalize(mV), 
				optix::normalize(-mW), 
				mLookat);
		const optix::Matrix4x4 frameInv = frame.inverse();
		const optix::Matrix4x4 trans =
			frame * mTransMat * mTransMat * frameInv;
		mEye = optix::make_float3(trans * optix::make_float4(mEye, 1.0f));
		mLookat = optix::make_float3(trans * optix::make_float4(mLookat, 1.0f));
		mUp = optix::make_float3(trans* optix::make_float4(mUp, 1.0f));
		calculateCamParams();
	}
}
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_PINHOLE_CAM_PROGRAM_H

