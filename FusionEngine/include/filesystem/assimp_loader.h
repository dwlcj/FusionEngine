#ifndef _INCLUDE_FILESYSTEM_ASSIMP_LOADER_H_
#define _INCLUDE_FILESYSTEM_ASSIMP_LOADER_H_

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
//
#include <plog/Log.h>
//
#include <filesystem/scene_loader_adapter.h>
//
#include <memory>
#include <filesystem>
namespace fs {
	class AssimpLoader : public SceneLoaderAdapter {
	public:
		explicit AssimpLoader();
		void loadFile(const std::string& filepath);
		void scanFile(const std::string& filepath);
		const bool isSupported(const std::string& extension);
	private:
		std::shared_ptr<Assimp::Importer> mAssimpImporter;
	};

	AssimpLoader::AssimpLoader() {
		mAssimpImporter = std::make_shared<Assimp::Importer>();
	}

	void AssimpLoader::loadFile(const std::string& filepath) {
		LOG_DEBUG << "Attempting to load file: " << filepath;
		const aiScene* scene = mAssimpImporter->ReadFile(filepath, aiProcess_ValidateDataStructure);
		if (!scene) {
			LOG_ERROR << "Error while reading file: " << filepath;
			LOG_ERROR << mAssimpImporter->GetErrorString();
		}
	}

}	// !namespace fs
#endif // !_INCLUDE_FILESYSTEM_ASSIMP_LOADER_H_

