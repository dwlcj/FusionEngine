#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H

#define SKATA
/// OptiX
#include <optixu/optixpp_namespace.h>
/// Logging
#include <plog/Log.h>
/// Fusion
#include <scene/scene.h>
#include <scene/basic_light.h>
#include <ptx_compiler/ptx_compiler.h>
#include <optix_rendering_system/optix_rendering_system.h>
#include <optix_scene_hierarchy/scene_root.h>
#include <optix_scene_hierarchy/scene_crown.h>
/// STL
#include <vector>
#include <functional>
#include <memory>
namespace tree {
	/**
	*	Scene Graph
	*/
	class SceneTree {
	public:
		explicit SceneTree(
			optix::Context& ctx, 
			const std::size_t& viewportWidth, 
			const std::size_t& viewportHeight);
		std::function<void(scene::Scene&)> sceneFlowIn();
		const std::shared_ptr<SceneCrown> crown() { return mCrown; }
		void render();
	protected:
		void addRoot(scene::Scene& s);
	private:
		unsigned int mCurLaunchIndex;
		std::size_t mLaunchIndexCount;
		optix::Buffer mOutputBuffer;
		optix::Context mContext;
		optix::Group mGroup;
		std::vector<SceneRoot> mRoots;
		std::shared_ptr<SceneCrown> mCrown;
		
		std::shared_ptr<rt::OptiXRenderer> mRenderer;
	};

	/**
	*	Constructor
	*/
	SceneTree::SceneTree(
		optix::Context& ctx,
		const std::size_t& viewportWidth, 
		const std::size_t& viewportHeight)
		: mContext(ctx)
	{
		mCrown = std::make_shared<SceneCrown>(ctx, viewportWidth, viewportHeight);
		mRenderer = 
			std::make_shared<rt::OptiXRenderer>(ctx, viewportWidth, viewportHeight);
		mGroup = mContext->createGroup();
		mGroup->setAcceleration(mContext->createAcceleration("Trbvh"));
		mContext["top_object"]->set(mGroup);
		mContext["top_shadower"]->set(mGroup);
		mCurLaunchIndex = 0u;
	}

	/**
	*	Converts Scene to Root
	*/
	std::function<void(scene::Scene& s)> SceneTree::sceneFlowIn() {
		return [this](scene::Scene& s) {
			addRoot(s);
		};
	}

	/**
	*	Adds Root
	*/
	void SceneTree::addRoot(scene::Scene& s) {
		mRoots.emplace_back(SceneRoot().createNew(mContext, s));
		mGroup->addChild(mRoots.back().topNode());
		mCurLaunchIndex = 1u;
	}

	/**
	*
	*/
	void SceneTree::render() {
		mRenderer->render(mCurLaunchIndex, mContext);
	}

	/*
	*	Adds a default basic light
	*/
	// TODO: make better lights


}	//	!namespace tree
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
