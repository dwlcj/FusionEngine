#ifndef INCLUDE_EDITOR_UI_MENU_BAR_H
#define INCLUDE_EDITOR_UI_MENU_BAR_H
/// ImGui
#include <imgui/imgui.h>
/// Fusion
#include <editor_ui/menu_item.h>
/// STL
#include <vector>
#include <memory>
namespace ui {

	class MenuBar {
	public:
		explicit MenuBar() { }
		void addItem(const std::shared_ptr<MenuBarItem>& item);
		void render();
	private:
		std::vector<std::shared_ptr<MenuBarItem>> mItems;
	};

	/**
	*	Add Menu Bar Item in the menu bar
	*/
	void MenuBar::addItem(const std::shared_ptr<MenuBarItem>& item) {
		mItems.emplace_back(item);
	}

	/**
	* Renders menu bar items
	*/
	void MenuBar::render() {
		if (ImGui::BeginMainMenuBar()) {
			for (std::size_t i = 0; i < mItems.size(); ++i) {
				mItems[i]->render();
			}
			ImGui::EndMainMenuBar();
		}
	}
		
	
}	//	!namespace ui
#endif // !INCLUDE_EDITOR_UI_MENU_BAR_H

