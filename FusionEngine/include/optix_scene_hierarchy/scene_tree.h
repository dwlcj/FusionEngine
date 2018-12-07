#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
/// OptiX
#include <optixu/optixpp_namespace.h>
/// Logging
#include <plog/Log.h>
/// Fusion
#include <scene/scene.h>
#include <optix_scene_hierarchy/scene_root.h>
/// STL
#include <vector>
#include <functional>
namespace tree {
	/**
	*	Scene Graph
	*/
	class SceneTree {
	public:
		explicit SceneTree() { }
		explicit SceneTree(optix::Context& ctx)
			: mContext(ctx)
		{ }
		const std::size_t rootCount() const { return mRoots.size(); }
		const std::vector<SceneRoot> roots() const { return mRoots; }
		const SceneRoot root(const std::size_t& idx) const;
		void addRoot(const SceneRoot& root);
		std::function<void(scene::Scene& s)> sceneFlowIn();
	protected:
		void addRoot(scene::Scene& s);
	private:
		optix::Context mContext;
		optix::Group mGroup;			///< This is the tree (top node)
		std::vector<SceneRoot> mRoots;
		// who has the lights? probaly this guy here.
		// Should the tree have the mapper?
		// It would be cleaner
	};

	/**
	*	adds root
	*/
	void SceneTree::addRoot(const SceneRoot& root) {
		mRoots.emplace_back(root);
	}

	/**
	*	returns indexed root
	*/
	const SceneRoot SceneTree::root(const std::size_t& idx) const {
		if (mRoots.empty()) {
			LOG_ERROR << "Tree has no roots.";
			return SceneRoot();
		}
		else if (idx < 0 || idx >= mRoots.size()) {
			LOG_ERROR << "Index out of range.";
			return SceneRoot();
		}
		return mRoots[idx];
	}

	/**
	*	maps fusion scene to root
	*	(adds a root)
	*/
	std::function<void(scene::Scene& s)> SceneTree::sceneFlowIn() {
		return [this](scene::Scene& s) {
			addRoot(s);
		};
	}

	/**
	*	maps fusion scene to root
	*/
	// TODO: versatile scene adding
	void SceneTree::addRoot(scene::Scene& s) {
		// compress scene to one mesh
		s.compress();
		scene::Mesh m = s.meshes().back();
		// get materials
		std::vector<scene::Material> mats = s.materials();
	}

}	//	!namespace tree
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
