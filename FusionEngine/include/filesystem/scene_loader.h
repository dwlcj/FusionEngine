#ifndef _INCLUDE_FILESYSTEM_SCENE_LOADER_H_
#define _INCLUDE_FILESYSTEM_SCENE_LOADER_H_

#include <string>

namespace fs {
	class SceneLoader {
	public:
		SceneLoader() { }
		virtual void loadFile(const std::string& filepath) = 0;
	};
}
#endif // !_INCLUDE_FILESYSTEM_SCENE_LOADER_H_