#ifndef INCLUDE_FILESYSTEM_SCENE_LOADER_ADAPTER_H
#define INCLUDE_FILESYSTEM_SCENE_LOADER_ADAPTER_H

#include <rxcpp/rx.hpp>
//
#include <filesystem/scene_loader.h>
#include <filesystem/assimp_loader.h>
//
#include <string>

namespace fs {
	/*! Implements SceneLoader Inerface
	*/
	class SceneLoaderAdapter : public SceneLoader, private AssimpLoader {
	public:
		SceneLoaderAdapter() { }
		void loadFile(const std::string& filepath) override;
		rxcpp::observable<scene::Scene> sceneFlowOut() override;
	private:
		rxcpp::subjects::subject<scene::Scene> mSceneFlowOut;
	};

	/*! Imports 3D file using Assimp
	*/
	void SceneLoaderAdapter::loadFile(const std::string& filepath) {
		const aiScene* scene = AssimpLoader::loadFile(filepath);
		scene::Scene fusionScene = AssimpLoader::importScene(scene);
		mSceneFlowOut.get_subscriber().on_next(fusionScene);
	}

	/*!	Returns fusionScene as observable for subscription
	*/
	rxcpp::observable<scene::Scene> SceneLoaderAdapter::sceneFlowOut() {
		return mSceneFlowOut.get_observable().as_dynamic();
	}


}	//	!namespace fs
#endif // !INCLUDE_FILESYSTEM_SCENE_LOADER_ADAPTER_H

