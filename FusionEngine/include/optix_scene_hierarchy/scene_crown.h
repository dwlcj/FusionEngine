#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_CROWN_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_CROWN_H
/// OptiX
#include <optixu/optixpp_namespace.h>
/// Fusion
#include <optix_scene_hierarchy/scene_camera.h>
/// STL
#include <memory>
#include <functional>
namespace tree {
	class SceneCrown {
	public:
		explicit SceneCrown() { }
		explicit SceneCrown(
			optix::Context& ctx, 
			const std::size_t& viewportWidth, 
			const std::size_t& viewportHeight);
		std::function<void(const std::tuple<float, optix::float3>&)> pinholeRotationIn();
	private:
		optix::float3 mSolidColor;
		optix::float3 mExceptionColor;
		optix::float3 mSimpleMissColor;
		const std::size_t mLaunchIndexCount = 2;
		optix::Program mSolidColorProgram;
		optix::Program mSimpleMissProgram;
		optix::Program mExceptionProgram;
		optix::Program mPinholeCamProgram;
		optix::Program mPanoCamProgram;
		std::shared_ptr<tree::SceneCamera> mPinholeCamera;
	};

	/**
	*	Constructor
	*/
	SceneCrown::SceneCrown(
		optix::Context& ctx, 
		const std::size_t& viewportWidth,
		const std::size_t& viewportHeight) {
		mPinholeCamera = std::make_shared<tree::SceneCamera>(ctx, viewportWidth, viewportHeight);
		mSolidColorProgram = ctx->createProgramFromPTXFile("res\\ptx\\draw_color.ptx", "draw_solid_color");
		mSimpleMissProgram = ctx->createProgramFromPTXFile("res\\ptx\\simple_miss.ptx", "miss");
		mExceptionProgram = ctx->createProgramFromPTXFile("res\\ptx\\exception.ptx", "exception");
		mPinholeCamProgram = ctx->createProgramFromPTXFile("res\\ptx\\pinhole_camera.ptx", "pinhole_camera");

		ctx->setRayTypeCount(2u);
		ctx->setEntryPointCount(2u);
		ctx->setRayGenerationProgram(0u, mSolidColorProgram);
		ctx->setRayGenerationProgram(1u, mPinholeCamProgram);
		ctx->setMissProgram(0u, mSimpleMissProgram);
		ctx->setMissProgram(1u, mSimpleMissProgram);
		ctx->setExceptionProgram(0u, mExceptionProgram);
		ctx->setExceptionProgram(1u, mExceptionProgram);

		mSolidColor = optix::make_float3(0.3f, 0.3f, 0.6f);
		mSimpleMissColor = optix::make_float3(0.1f, 0.8f, 0.1f);
		mExceptionColor = optix::make_float3(1.0f, 0.0f, 0.0f);
		mSolidColorProgram["draw_color"]->setFloat(mSolidColor);
		mSimpleMissProgram["bg_color"]->setFloat(mSimpleMissColor);
		mExceptionProgram["bad_color"]->setFloat(mExceptionColor);
		mPinholeCamProgram["eye"]->setFloat(mPinholeCamera->eye());
		mPinholeCamProgram["U"]->setFloat(mPinholeCamera->u());
		mPinholeCamProgram["V"]->setFloat(mPinholeCamera->v());
		mPinholeCamProgram["W"]->setFloat(mPinholeCamera->w());
	}

	/**
	*	Rotates camera
	*/
	std::function<void(const std::tuple<float, optix::float3>&)> SceneCrown::pinholeRotationIn() {
		return [this](const std::tuple<float, optix::float3>& rot) {
			mPinholeCamera->rotate(std::get<0>(rot), std::get<1>(rot));
			mPinholeCamProgram["eye"]->setFloat(mPinholeCamera->eye());
			mPinholeCamProgram["U"]->setFloat(mPinholeCamera->u());
			mPinholeCamProgram["V"]->setFloat(mPinholeCamera->v());
			mPinholeCamProgram["W"]->setFloat(mPinholeCamera->w());
		};
	}

	
}
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_CROWN_H

