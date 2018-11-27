#ifndef _INCLUDE_FILESYSTEM_SCENE_LOADER_ADAPTER_H_
#define _INCLUDE_FILESYSTEM_SCENE_LOADER_ADAPTER_H_

#include <rxcpp/rx.hpp>
//
#include <filesystem\scene_loader.h>
#include <filesystem\assimp_loader.h>
//
#include <string>
#include <memory>
#include <filesystem>

namespace fs {
	/*! Implements SceneLoader Inerface
	*/
	class SceneLoaderAdapter : public SceneLoader, private AssimpLoader {
	public:
		SceneLoaderAdapter() { }
		void loadFile(const std::string& filepath) override;
		rxcpp::observable<scene::Scene> fusionSceneOut();
	private:
		rxcpp::subjects::subject<scene::Scene> mSceneOut;
	};

	/*! Imports 3D file using Assimp
	*/
	void SceneLoaderAdapter::loadFile(const std::string& filepath) {
		const aiScene* scene = AssimpLoader::loadFile(filepath);
		const scene::Scene fusionScene = AssimpLoader::importScene(scene);
		mSceneOut.get_subscriber().on_next(fusionScene);
	}

	/*!	Returns fusionScene as observable for subscription
	*/
	rxcpp::observable<scene::Scene> SceneLoaderAdapter::fusionSceneOut() {
		return mSceneOut.get_observable().as_dynamic();
	}


}	//	!namespace fs
#endif // !_INCLUDE_FILESYSTEM_SCENE_LOADER_ADAPTER_H_

