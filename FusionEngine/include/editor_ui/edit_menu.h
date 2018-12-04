#ifndef INCLUDE_EDITOR_UI_EDIT_MENU_H
#define INCLUDE_EDITOR_UI_EDIT_MENU_H
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
#endif // !INCLUDE_EDITOR_UI_EDIT_MENU_H

