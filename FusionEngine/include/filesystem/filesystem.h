#ifndef INCLUDE_FILESYSTEM_FILESYSTEM_H
#define INCLUDE_FILESYSTEM_FILESYSTEM_H
/// RX
#include <rxcpp/rx.hpp>
/// Fusion
#include <filesystem/scene_loader.h>
#include <filesystem/scene_loader_adapter.h>
#include <comm_system/filesystem_message.h>
/// STL
#include <memory>
#include <functional>
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
		std::function<void(const comm::FilesystemMessage&)> messageFlowIn();
		rxcpp::observable<scene::Scene> sceneFlowOut();
	private:
		std::shared_ptr<fs::SceneLoader> mSceneLoader;
		// VideoLoader
		// PTX loader
		// StateLoader?
	};

	// Constructor
	Filesystem::Filesystem() {
		mSceneLoader = std::make_shared<SceneLoaderAdapter>();
	}
	
	// loads 3D file
	void Filesystem::load3DFile(const std::string& filepath) {
		mSceneLoader->loadFile(filepath);
	}

	/**
	*	message flow in functor
	*/
	std::function<void(const comm::FilesystemMessage&)> Filesystem::messageFlowIn() {
		return [this](const comm::FilesystemMessage& message) {
			std::string filepath = message.filepath();
			comm::ActionType action = message.action();
			comm::FileType type = message.fileType();
			if (action == comm::ActionType::Open) {
				switch (type)
				{
				case comm::FileType::Graphics:
					load3DFile(filepath);
					break;
				case comm::FileType::PerfCap:
					// Load Perfcap file
					LOG_DEBUG << "Prf. Cap. loading os not implemented at the moment";
					break;
				case comm::FileType::Image:
					// Load Perfcap file
					LOG_DEBUG << "Image loading os not implemented at the moment";
					break;
				case comm::FileType::Project:
					// Load Perfcap file
					LOG_DEBUG << "Project loading os not implemented at the moment";
					break;
				case comm::FileType::Video:
					// Load Perfcap file
					LOG_DEBUG << "Video loading os not implemented at the moment";
					break;
				default:
					LOG_DEBUG << "Should not delegate to default.";
					break;
				}
			}
			else if (action == comm::ActionType::Save) {
				switch (type)
				{
				case comm::FileType::Graphics:
					LOG_DEBUG << "Saving is not implemented at the moment";
					break;
				case comm::FileType::PerfCap:
					// Load Perfcap file
					LOG_DEBUG << "Savings not implemented at the moment";
					break;
				case comm::FileType::Image:
					// Load Perfcap file
					LOG_DEBUG << "Saving is not implemented at the moment";
					break;
				case comm::FileType::Project:
					// Load Perfcap file
					LOG_DEBUG << "Saving is not implemented at the moment";
					break;
				case comm::FileType::Video:
					// Load Perfcap file
					LOG_DEBUG << "Saving is not implemented at the moment";
					break;
				default:
					LOG_DEBUG << "Should not delegate to default.";
					break;
				}
			}
			else {
				LOG_ERROR << "Filesystem: Unknown Action";
			}
		};
	}

	/**
	*
	*/
	rxcpp::observable<scene::Scene> Filesystem::sceneFlowOut() {
		return mSceneLoader->sceneFlowOut();
	}

}	// !namespace fs
#endif	//!INCLUDE_FILESYSTEM_FILESYSTEM_H
