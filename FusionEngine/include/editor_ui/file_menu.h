#ifndef INCLUDE_EDITOR_UI_FILE_MENU_H
#define INCLUDE_EDITOR_UI_FILE_MENU_H
/// ImGui
#include <imgui/imgui.h>
/// Fusion
#include <editor_ui/menu_item.h>
#include <editor_ui/file_eplorer_popup.h>
/// STL
#include <memory>
namespace ui {
	/**
	*	FileMenu Class
	*	Handles File Loading and saving 
	*	and communicates with the Filesystem
	*/
	class FileMenu : public MenuBarItem {
	public:
		FileMenu();
		void render() override;
	private:
		std::shared_ptr<ui::pop::FileExplorerPopup> mFileExplorerPopup;
	};

	FileMenu::FileMenu() {
		mFileExplorerPopup = std::make_shared<ui::pop::FileExplorerPopup>();
	}

	void FileMenu::render() {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open Project...")) {
				mFileExplorerPopup->pop();
			}
			if (ImGui::MenuItem("Save Project...")) {

			}
			if (ImGui::MenuItem("Save Project as...")) {

			}
			if (ImGui::MenuItem("Import 3D file...")) {

			}
			if (ImGui::MenuItem("Import Video file...")) {

			}
			if (ImGui::MenuItem("Import PerfCap file(s)...")) {

			}
			ImGui::EndMenu();
		}
	}
}	//	!namespace ui
#endif // !INCLUDE_EDITOR_UI_FILE_MENU_H

