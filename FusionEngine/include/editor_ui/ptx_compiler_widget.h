#ifndef _INCLUDE_EDITOR_UI_PTX_COMPILER_WIDGET_H_
#define _INCLUDE_EDITOR_UI_PTX_COMPILER_WIDGET_H_
/// ImGui
#include <imgui/imgui.h>
/// Fusion
#include <editor_ui/widget.h>
/// STL
#include <string>
namespace ui {
	namespace widget {
		
		class PTXCompilerWidget : public Widget {
			explicit PTXCompilerWidget() { }
			void render() override;
		private:
			// How I can achieve communication?
			// Probaly via RX bu we'll see
			const char* const mCudaArchs[3] = { "30", "51", "52"};
			int mSelCudaArch = 0;
		};

		void PTXCompilerWidget::render() {
			ImGui::Begin("PTX Compiler");
			{
				ImGui::Combo("Cuda Architecture##", &mSelCudaArch, mCudaArchs, 3);

			}
			ImGui::End();
		}

	}	//	!namespace widget
}	//	!namespace ui
#endif // !_INCLUDE_EDITOR_UI_PTX_COMPILER_WIDGET_H_

