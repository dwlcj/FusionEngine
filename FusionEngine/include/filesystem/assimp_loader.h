#ifndef _INCLUDE_FILESYSTEM_ASSIMP_LOADER_H_
#define _INCLUDE_FILESYSTEM_ASSIMP_LOADER_H_

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// Logging
#include <plog/Log.h>

// FusionEngine
#include <filesystem/file_load_adapter.h> 

//stl
#include <string>
#include <memory>
namespace fs {
	// Loads 3D files using Assimp
	class AssimpLoader : public FileLoadAdapter {
	public:
		explicit AssimpLoader() { }
		void loadFile(const std::string& filepath);
	protected:
		// protected functions
		void isSupported();
		void scanFile();
	private:
		std::shared_ptr<Assimp::Importer> mAssimpImporter;
	};
}	// !namespace fs
#endif // !_INCLUDE_FILESYSTEM_ASSIMP_LOADER_H_

