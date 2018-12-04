#ifndef _INCLUDE_EDITOR_UI_EDIT_MENU_H_
#define _INCLUDE_EDITOR_UI_EDIT_MENU_H_
/// ImGui
#include <imgui/imgui.h>
/// Fusion
#include <editor_ui/menu_item.h>

namespace ui {
	class EditMenu : public MenuBarItem {
	public:
		explicit EditMenu() { }
		void render() override;
	};

	void EditMenu::render() {
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo")) {

			}
			if (ImGui::MenuItem("Redo")) {

			}
			if (ImGui::MenuItem("Copy")) {

			}
			if (ImGui::MenuItem("Paste")) {

			}
			ImGui::EndMenu();
		}
	}
}
#endif // !_INCLUDE_EDITOR_UI_EDIT_MENU_H_

