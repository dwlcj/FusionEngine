#ifndef _INCLUDE_FILESYSTEM_SCENE_LOADER_ADAPTER_H_
#define _INCLUDE_FILESYSTEM_SCENE_LOADER_ADAPTER_H_

//
#include <string>
#include <memory>
#include <filesystem>

namespace fs {

	class SceneLoaderAdapter {
	public:
		virtual void loadFile(const std::string& filepath) = 0;
		virtual void scanFile(const std::string& filepath) = 0;
	private:
	};



}	//	!namespace fs
#endif // !_INCLUDE_FILESYSTEM_SCENE_LOADER_ADAPTER_H_

