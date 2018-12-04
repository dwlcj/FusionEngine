#ifndef INCLUDE_FILESYSTEM_FILESYSTEM_H
#define INCLUDE_FILESYSTEM_FILESYSTEM_H
/// Fusion
#include <filesystem/scene_loader.h>
#include <filesystem/scene_loader_adapter.h>
/// STL
#include <memory>

namespace fs {

	// Filesystem class
	class Filesystem {
	public:
		explicit Filesystem();
		void load3DFile(const std::string& filepath);
		// TODO: Maybe ptx file loading is not needed in the filesystem because
		// OptiX can load ptx files itself
		void loadPTXFile(const std::string& filepath);
		// Load 3D files
		// Load Video files
		// Load/Save Project's state
	private:
		std::shared_ptr<fs::SceneLoader> sceneLoader;
		// VideoLoader
		// PTX loader
		// StateLoader?
	};

	// Constructor
	Filesystem::Filesystem() {
		sceneLoader = std::make_shared<SceneLoaderAdapter>();
	}
	
	// loads 3D file
	void Filesystem::load3DFile(const std::string& filepath) {
		sceneLoader->loadFile(filepath);
	}

}	// !namespace fs
#endif	//!INCLUDE_FILESYSTEM_FILESYSTEM_H
