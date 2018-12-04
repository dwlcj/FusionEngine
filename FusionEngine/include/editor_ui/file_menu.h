#ifndef _INCLUDE_EDITOR_UI_FILE_MENU_H_
#define _INCLUDE_EDITOR_UI_FILE_MENU_H_
/// ImGui
#include <imgui/imgui.h>
/// Fusion
#include <editor_ui/menu_item.h>

namespace ui {
	/**
	*	FileMenu Class
	*	Handles File Loading and saving 
	*	and communicates with the Filesystem
	*/
	class FileMenu : public MenuBarItem {
	public:
		FileMenu() { }
		void render() override;
	private:
	};

	void FileMenu::render() {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open Project...")) {

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
#endif // !_INCLUDE_EDITOR_UI_FILE_MENU_H_

