#ifndef INCLUDE_FILESYSTEM_SCENE_LOADER_H
#define INCLUDE_FILESYSTEM_SCENE_LOADER_H
///RX
#include <rxcpp/rx.hpp>
#include <scene/scene.h>
/// STL
#include <string>
namespace fs {
	class SceneLoader {
	public:
		SceneLoader() { }
		virtual void loadFile(const std::string& filepath) = 0;
		virtual rxcpp::observable<scene::Scene> sceneFlowOut() = 0;
	};
}
#endif // !INCLUDE_FILESYSTEM_SCENE_LOADER_H