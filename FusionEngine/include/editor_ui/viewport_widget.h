#ifndef INCLUDE_EDITOR_UI_VIEWPORT_WIDGET_H
#define INCLUDE_EDITOR_UI_VIEWPORT_WIDGET_H
/// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
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
			explicit ViewportWidget();
			void render() override;
			rxcpp::observable<comm::OptiXRendererMessage> messageFlowOut();
			rxcpp::observable<std::tuple<float, optix::float3>> pinholeRotationOut();
		private:
			rxcpp::subjects::subject<comm::OptiXRendererMessage> mMessageFlowOut;
			bool mNone = true;
			bool mPinhole = false;
			bool mPano = false;
			optix::float3 mRotation;
			rxcpp::subjects::subject<std::tuple<float, optix::float3>> mPinholeRotOut;
		};

		/**
		*	Viewport
		*/
		ViewportWidget::ViewportWidget() {
			mRotation = optix::make_float3(0.0f, 0.0f, 0.0f);
		}

		/**
		*	Exposes message flow out for subscription
		*/
		rxcpp::observable<comm::OptiXRendererMessage> ViewportWidget::messageFlowOut() {
			return mMessageFlowOut.get_observable().as_dynamic();
		}

		/**
		*
		*/
		rxcpp::observable<std::tuple<float, optix::float3>> ViewportWidget::pinholeRotationOut() {
			return mPinholeRotOut.get_observable().as_dynamic();
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
				ImGui::PushItemWidth(100);
				if (ImGui::SliderFloat("RotateX##", &mRotation.x, 0, 3.14)) {
					mPinholeRotOut.get_subscriber().on_next(std::make_tuple(mRotation.x, optix::make_float3(1.0f, 0.0f, 0.0f)));
				}
				ImGui::SameLine();
				if (ImGui::SliderFloat("RotateY##", &mRotation.y, 0, 3.14)) {
					mPinholeRotOut.get_subscriber().on_next(std::make_tuple(mRotation.y, optix::make_float3(0.0f, 1.0f, 0.0f)));
				}
				ImGui::SameLine();
				if (ImGui::SliderFloat("RotateZ##", &mRotation.z, 0, 3.14)) {
					mPinholeRotOut.get_subscriber().on_next(std::make_tuple(mRotation.z, optix::make_float3(0.0f, 0.0f, 1.0f)));
				}
				ImGui::PopItemWidth();
			}
			ImGui::End();
		}

	}	//	!namespace widget
}	//	!namespace ui
#endif // !INCLUDE_EDITOR_UI_VIEWPORT_WIDGET_H

