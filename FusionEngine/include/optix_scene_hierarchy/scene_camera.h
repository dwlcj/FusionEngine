#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_CAMERA_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_CAMERA_H
/// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
/// STL
#include <vector>
namespace tree {
	class SceneCamera {
	public:
		explicit SceneCamera() { }
		explicit SceneCamera(
			optix::Context& ctx, 
			const std::size_t& viewportWidth, 
			const std::size_t& viewPortHeight);
		const optix::float3 eye() const { return mEye;}
		const optix::float3 lookat() const { return mLookat; }
		const optix::float3 up() const { return mUp; }
		const optix::float3 u() const { return mU; }
		const optix::float3 v() const { return mV; }
		const optix::float3 w() const { return mW; }
		void rotate(const float& rad, const optix::float3& axis);
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
	SceneCamera::SceneCamera(
		optix::Context& ctx,
		const std::size_t& viewportWidth,
		const std::size_t& viewportHeight)
	{
		mEye = optix::make_float3(0.0f, 0.0f, 0.0f);
		mLookat = optix::make_float3(0.0f, 0.0f, 1.0f);
		mUp = optix::make_float3(0.0f, -1.0f, 0.0f);
		mFOV = 35.0f;
		mAspectRatio = static_cast<float>(mViewportWidth) / static_cast<float>(mViewportHeight);
		mTransMat = optix::Matrix4x4::identity();
		updateParams();
	}

	void SceneCamera::rotate(const float& rad, const optix::float3& axis) {
		mTransMat.rotate(rad, axis);
		updateParams();
	}

	/**
	*	Calculates Camera space coordinates (U, V, W)
	*/
	void SceneCamera::calculateCamParams() {
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
	void SceneCamera::updateParams() {
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
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_CAMERA_H

