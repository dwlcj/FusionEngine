#ifndef INCLUDE_EDITOR_UI_VIEWPORT_WIDGET_H
#define INCLUDE_EDITOR_UI_VIEWPORT_WIDGET_H
///ImGui
#include <imgui/imgui.h>
/// RX
#include <rxcpp/rx.hpp>
/// Fusion
#include <editor_ui/widget.h>
#include <comm_system/optix_renderer_message.h>

namespace ui {
	namespace widget {

		class ViewportWidget : public Widget {
		public:
			explicit ViewportWidget() { }
			void render() override;
			rxcpp::observable<comm::OptiXRendererMessage> messageFlowOut();
		private:
			rxcpp::subjects::subject<comm::OptiXRendererMessage> mMessageFlowOut;
			bool mNone = true;
			bool mPinhole = false;
			bool mPano = false;
		};

		/**
		*	Exposes message flow out for subscription
		*/
		rxcpp::observable<comm::OptiXRendererMessage> ViewportWidget::messageFlowOut() {
			return mMessageFlowOut.get_observable().as_dynamic();
		}

		/**
		*	Renders widget on screen
		*/
		void ViewportWidget::render() {
			ImGui::Begin("Viewport Controller");
			{
				if (ImGui::Checkbox("None", &mNone)) {
					mPinhole = false;
					mPano = false;
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Perspective", &mPinhole)) {
					mNone = false;
					mPano = false;
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Panoramic", &mPano)) {
					mNone = false;
					mPinhole = false;
				}
			}
			ImGui::End();
		}

	}	//	!namespace widget
}	//	!namespace ui
#endif // !INCLUDE_EDITOR_UI_VIEWPORT_WIDGET_H

