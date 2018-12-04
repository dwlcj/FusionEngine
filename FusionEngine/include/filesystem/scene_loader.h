#ifndef INCLUDE_FILESYSTEM_SCENE_LOADER_H
#define INCLUDE_FILESYSTEM_SCENE_LOADER_H

#include <string>

namespace fs {
	class SceneLoader {
	public:
		SceneLoader() { }
		virtual void loadFile(const std::string& filepath) = 0;
	};
}
#endif // !INCLUDE_FILESYSTEM_SCENE_LOADER_H