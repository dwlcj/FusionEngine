#ifndef INCLUDE_EDITOR_UI_FILE_EXPLORER_POPUP_H
#define INCLUDE_EDITOR_UI_FILE_EXPLORER_POPUP_H
/// ImGui
#include <imgui/imgui.h>
/// RX
#include <rxcpp/rx.hpp>
/// Logging
#include <plog/Log.h>
/// Fusion
#include <editor_ui/pop_up.h>
/// STL
#include <vector>
#include <filesystem>
namespace ui {
	namespace pop {
		namespace fs = std::experimental::filesystem;

		/**
		*	File Explorer
		*/
		class FileExplorerPopup : public PopUp {
		public:
			explicit FileExplorerPopup();
			void pop() override;
			rxcpp::observable<std::string> pathFlowOut();
		protected:
			const std::vector<fs::path> listDir(const fs::path& dir);
			const std::vector<std::string> splitPath(const fs::path& path);
		private:
			fs::path mCurPath;
			fs::path mSelPath;
			rxcpp::subjects::subject<std::string> mSelPathOut;
		};

		/**
		*	constructor, gets and set's current path
		*/
		FileExplorerPopup::FileExplorerPopup() {
			mCurPath = fs::current_path();
		}

		/**
		*	lists items in a directory
		*/
		const std::vector<fs::path> listDir(const fs::path& dir) {
			if (dir.has_filename()) {
				LOG_DEBUG << "Not a directory.";
			}
			if (!dir.has_filename()) {
				std::vector<fs::path> pathList;
				//dir = std::experimental::filesystem::directory
				for (auto& item : fs::directory_iterator(dir)) {
					pathList.emplace_back(item);
				}
				return pathList;
			}
		}

		/**
		*	Splits path and returns indivual values
		*/
		const std::vector<std::string> FileExplorerPopup::splitPath(const fs::path& path) {
			std::vector<std::string> pathElements;
			for (fs::path::const_iterator it = path.begin(); it != path.end(); ++it) {
				pathElements.emplace_back(it->generic_string());
			}
			return pathElements;
		}

		/**
		*	Pops popup on screen
		*/
		void FileExplorerPopup::pop() {
			ImGui::BeginPopup("File Explorer");
			{
				if (ImGui::BeginPopupModal("File Explorer")) {
					std::vector<std::string> pathElements = splitPath(mCurPath);
					for (std::vector<std::string>::const_iterator it = pathElements.begin();
						it != pathElements.end(); ++it) {
						bool sel;
						if (ImGui::Selectable(it->c_str(), &sel)) {

						}
					}
					if (ImGui::Button("Cancel##")) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				ImGui::EndPopup();
			}
			
		
			
		}
	
	}	//	!namespace pop
}	//	!namespace ui
#endif // !INCLUDE_EDITOR_UI_FILE_EXPLORER_POPUP_H

