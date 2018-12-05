#ifndef INCLUDE_EDITOR_UI_FILE_MENU_H
#define INCLUDE_EDITOR_UI_FILE_MENU_H
/// ImGui
#include <imgui/imgui.h>
/// Fusion
#include <editor_ui/menu_item.h>
#include <editor_ui/file_explorer_popup.h>
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
		rxcpp::observable<comm::FilesystemMessage> messageFlowOut();
	private:
		std::shared_ptr<ui::pop::FileExplorerPopup> mFileExplorerPopup;
	};

	/**
	*	Initializes FileExplorer PopUp
	*/
	FileMenu::FileMenu() {
		mFileExplorerPopup = std::make_shared<ui::pop::FileExplorerPopup>();
	}

	/**
	*	Renders Gui Logic
	*/
	void FileMenu::render() {
		bool openProjectFlag = false;
		bool saveProjectFlag = false;
		bool saveProjectAsFlag = false;
		bool import3DFileFlag = false;
		bool importVideoFileFlag = false;
		bool importPerfCapFileFlag = false;

		if (ImGui::BeginMenu("File")) {
			openProjectFlag = ImGui::MenuItem("Open Project...");
			saveProjectFlag = ImGui::MenuItem("Save Project...");
			saveProjectAsFlag = ImGui::MenuItem("Save Project as...");
			import3DFileFlag = ImGui::MenuItem("Import 3D File...");
			importVideoFileFlag = ImGui::MenuItem("Import Video File...");
			importPerfCapFileFlag = ImGui::MenuItem("Import Perf. Cap. File(s)...");
			ImGui::EndMenu();
		}
		// File Explorer Popup
		mFileExplorerPopup->pop(openProjectFlag, comm::ActionType::Open, comm::FileType::Project);
		mFileExplorerPopup->pop(saveProjectFlag, comm::ActionType::Save, comm::FileType::Project);
		mFileExplorerPopup->pop(import3DFileFlag, comm::ActionType::Open, comm::FileType::Graphics);
		mFileExplorerPopup->pop(importVideoFileFlag, comm::ActionType::Open, comm::FileType::Video);
		mFileExplorerPopup->pop(importPerfCapFileFlag, comm::ActionType::Open, comm::FileType::PerfCap);
	}

	/**
	*	Exposes FileExplorer's message Flow out for subscription
	*/
	rxcpp::observable<comm::FilesystemMessage> FileMenu::messageFlowOut() {
		return mFileExplorerPopup->messageFlowFlowOut();
	}
}	//	!namespace ui
#endif // !INCLUDE_EDITOR_UI_FILE_MENU_H

