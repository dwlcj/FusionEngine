#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
/// OptiX
#include <optixu/optixpp_namespace.h>
/// Logging
#include <plog/Log.h>
/// Fusion
#include <scene/scene.h>
#include <ptx_compiler/ptx_compiler.h>
#include <optix_scene_hierarchy/scene_root.h>
#include <optix_rendering_system/bounding_box_program.h>
#include <optix_rendering_system/intersection_program.h>
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
		explicit SceneTree() { }
		explicit SceneTree(optix::Context& ctx)
			: mContext(ctx)
		{ }
		explicit SceneTree(
			optix::Context& ctx, 
			const std::shared_ptr<ptx::PTXCompiler> compiler);
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
		std::shared_ptr<rt::BoundingBoxProgram> mBoundingBoxProgram;
		std::shared_ptr<rt::IntersectionProgram> mIntersectionProgram;
		// TODO: make closest hit and anyhit program type objects
		// who has the lights? probaly this guy here.
		// Should the tree have the mapper?
		// It would be cleaner
	};

	/**
	*	Constructor
	*/
	SceneTree::SceneTree(
		optix::Context& ctx,
		const std::shared_ptr<ptx::PTXCompiler> compiler)
		: mContext(ctx)
	{
		mBoundingBoxProgram =
			std::make_shared<rt::BoundingBoxProgram>(
				ctx,
				std::string("mesh_bounds"),
				std::string("res\\ptx\\triangle_mesh.ptx"),
				std::string("..\\src\\CUDA\\triangle_mesh.cu"),
				compiler);
		mIntersectionProgram =
			std::make_shared<rt::IntersectionProgram>(
				ctx,
				std::string("mesh_intersect"),
				std::string("res\\ptx\\triangle_mesh.ptx"),
				std::string("..\\src\\CUDA\\triangle_mesh.cu"),
				compiler);
	}

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
		// map primitives
		// first make a Geometry Component

	}

}	//	!namespace tree
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_TREE_H
