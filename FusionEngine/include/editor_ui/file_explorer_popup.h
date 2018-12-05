#ifndef INCLUDE_EDITOR_UI_FILE_EXPLORER_POPUP_H
#define INCLUDE_EDITOR_UI_FILE_EXPLORER_POPUP_H
/// ImGui
#include <imgui/imgui.h>
#include <imgui/dirent_portable.h>
/// RX
#include <rxcpp/rx.hpp>
/// Logging
#include <plog/Log.h>
/// Fusion
#include <editor_ui/pop_up.h>
#include <comm_system/filesystem_message.h>
/// STL
#include <vector>
#include <filesystem>
namespace ui {
	namespace pop {
		namespace fs = std::experimental::filesystem;

		/**
		*	File Explorer
		*/
		class FileExplorerPopup : public PopUp<comm::ActionType, comm::FileType> {
		public:
			explicit FileExplorerPopup();
			void activate() override;
			void pop(const bool& pop, const comm::ActionType& action, const comm::FileType& filetype) override;
			const bool isActive() const { return mActive; }
			rxcpp::observable<comm::FilesystemMessage> messageFlowFlowOut();
		protected:
			const std::string genID(const comm::ActionType& action, const comm::FileType& filetype);
			const std::vector<fs::path> listDir(const fs::path& dir);
			const std::vector<fs::path> splitPath(const fs::path& path);
			void moveInside(const fs::path& dir);
			void moveUp();
		private:
			const std::string mName = "File Explorer";
			bool mActive = false;
			fs::path mCurPath;
			fs::path mSelPath;
			std::vector<fs::path> mCurPathElements;
			std::vector<fs::path> mCurPathItems;
			ImGuiWindowFlags mWindowFlags;
			ImVec2 mPopupSize;
			rxcpp::subjects::subject<comm::FilesystemMessage> mMessageFlowOut;
		};

		/**
		*	constructor, gets and set's current path
		*/
		FileExplorerPopup::FileExplorerPopup() {
			mCurPath = fs::current_path();
			mCurPathElements = splitPath(mCurPath);
			mCurPathItems = listDir(mCurPath);
			//mWindowFlags |= ImGuiWindowFlags_ResizeFromAnySide;
			//mWindowFlags |= ImGuiWindowFlags_NoScrollbar;
			mPopupSize = ImVec2(640, 480);
		}

		/**
		*	lists items in a directory
		*/
		const std::vector<fs::path> FileExplorerPopup::listDir(const fs::path& dir) {
			fs::path absPath = dir;
			std::vector<fs::path> pathList;
			if (fs::is_regular_file(dir)) {
				LOG_DEBUG << "Not a directory.";
			}
			if (fs::is_directory(dir)) {
				fs::directory_iterator dirIt(dir);
				for (auto& item : dirIt) {
					pathList.emplace_back(item);
				}
			}
			return pathList;
		}

		/**
		*	Splits path and returns indivual values
		*/
		const std::vector<fs::path> FileExplorerPopup::splitPath(const fs::path& path) {
			std::vector<fs::path> pathElements;
			fs::path absPath;
			// TODO: check the double slash near the disk
			for (fs::path::const_iterator it = path.begin(); it != path.end(); ++it) {
				absPath.append(*it);
				pathElements.emplace_back(absPath);
			}
			return pathElements;
		}


		/**
		*	Activates PopUp
		*/
		void FileExplorerPopup::activate() {
			mActive = true;
		}

		/**
		*	Moves up the directory hierarchy
		*/
		void FileExplorerPopup::moveUp() {
			if (mCurPath.has_parent_path()) {
				mCurPath = mCurPath.parent_path();
				mCurPathElements = splitPath(mCurPath);
				mCurPathItems = listDir(mCurPath);
			}
		}

		/**
		*	Moves in directory
		*/
		void FileExplorerPopup::moveInside(const fs::path& dir) {
			if (!fs::is_regular_file(dir)) {
				mCurPath.clear();
				mCurPath = dir;
				mCurPathElements = splitPath(mCurPath);
				mCurPathItems = listDir(mCurPath);
			}
		}

		/**
		*	Creates unique id for the Popup
		*/
		const std::string FileExplorerPopup::genID(const comm::ActionType& action, const comm::FileType& filetype) {
			std::string typeStr;
			std::string actionStr;
			switch (action)
			{
			case comm::ActionType::Open:
				actionStr = std::string("Open");
				break;
			case comm::ActionType::Save:
				actionStr = std::string("Save");
			default:
				actionStr = std::string("None");
				break;
			}
			switch (filetype)
			{
			case comm::FileType::Graphics:
				typeStr = std::string("3DFile");
				break;
			case comm::FileType::Image:
				typeStr = std::string("Image");
				break;
			case comm::FileType::Project:
				typeStr = std::string("Project");
				break;
			case comm::FileType::Video:
				typeStr = std::string("Video");
				break;
			case comm::FileType::PerfCap:
				typeStr = std::string("PerfCap");
				break;
			default:
				typeStr = std::string("None");
				break;
			}
			return std::string("##" + actionStr + typeStr);
		}

		/**
		*	Pops popup on screen
		*/
		void FileExplorerPopup::pop(const bool& pop, const comm::ActionType& action, const comm::FileType& filetype) {
			std::string ID = genID(action, filetype);
			std::string Name = mName + ID;
			// Open Popup
			if (pop) {
				ImGui::OpenPopup(Name.c_str());
			}
			// Popup Content
			ImGui::SetNextWindowSize(mPopupSize, ImGuiCond_FirstUseEver);
			if (ImGui::BeginPopupModal(Name.c_str())) {
				// TODO change up with arrow
				if (ImGui::Button("Up")) {
					moveUp();
				}
				// TODO: find logic to move inside
				for (fs::path& pathElem : mCurPathElements) {
					ImGui::SameLine();
					if (ImGui::Button(pathElem.filename().generic_string().c_str())) {
						moveInside(pathElem);
						break;
					}
					
				}
				ImGui::Separator();
				// Directory Items
				ImGui::BeginChild("Directory items");
				{
					// Display current dir's items
					for (fs::path& item : mCurPathItems) {
						if (ImGui::Selectable(item.filename().generic_string().c_str())) {
							if (fs::is_directory(item)) {
								moveInside(item);
								break;
							}
							else if (fs::is_regular_file(item)) {
								mSelPath = item;
								break;
							}
						}
					}
				}
				ImGui::EndChild();
				if (action == comm::ActionType::Open) {
					if (ImGui::Button("Open##")) {
						// generate message
						comm::FilesystemMessage message(mSelPath.generic_string(), action, filetype);
						// send to subscriber
						mMessageFlowOut.get_subscriber().on_next(message);
						ImGui::CloseCurrentPopup();
					}
				}
				else if (action == comm::ActionType::Save) {
					if (ImGui::Button("Save##")) {
						// generate message
						comm::FilesystemMessage message(mSelPath.generic_string(), action, filetype);
						// send to subscriber
						mMessageFlowOut.get_subscriber().on_next(message);
						ImGui::CloseCurrentPopup();
					}
				}
				
				ImGui::SameLine();
				if (ImGui::Button("Cancel##")) {
					mCurPath = fs::current_path();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}

		/**
		*	Exposes message flow out for subscription
		*/
		rxcpp::observable<comm::FilesystemMessage> FileExplorerPopup::messageFlowFlowOut() {
			return mMessageFlowOut.get_observable().as_dynamic();
		}
	
	}	//	!namespace pop
}	//	!namespace ui
#endif // !INCLUDE_EDITOR_UI_FILE_EXPLORER_POPUP_H

