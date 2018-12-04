#ifndef INCLUDE_EDITOR_UI_PTX_COMPILER_WIDGET_H
#define INCLUDE_EDITOR_UI_PTX_COMPILER_WIDGET_H
/// ImGui
#include <imgui/imgui.h>
/// rx
#include <rxcpp/rx.hpp>
/// Fusion
#include <editor_ui/widget.h>
#include <comm_system/ptx_compiler_message.h>
/// STL
#include <string>
namespace ui {
	namespace widget {
		
		class PTXCompilerWidget : public Widget {
		public:
			explicit PTXCompilerWidget();
			void render() override;
			rxcpp::observable<comm::PTXCompilerMessage> messageFlowOut();
		private:
			// How I can achieve communication?
			// Probaly via RX bu we'll see
			const char* const mCudaArchs[3] = { "30", "51", "52"};
			const char* const mPlatforms[2] = { "x86", "x64" };
			int mSelCudaArch = 0;
			int mSelPlatform = 1;
			bool mRdc = true;
			bool mFastMath = true;
			ImGuiWindowFlags mWindowFlags = 0;
			rxcpp::subjects::subject<comm::PTXCompilerMessage> mMessageFlowOut;
		};

		/**
		*	Constructor
		*	Sets Widget Flags
		*/
		PTXCompilerWidget::PTXCompilerWidget() {
			mWindowFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar;
		}

		/**
		*	ptx compiler message flow out
		*	Exposes mMessageFlowOut for subscription
		*/
		rxcpp::observable<comm::PTXCompilerMessage> PTXCompilerWidget::messageFlowOut() {
			return mMessageFlowOut.get_observable().as_dynamic();
		}

		/**
		*	Widget's render method
		*	Renders UI behaviour
		*/
		// TODO: Find another structure for sending individual messages
		//		 and don't construct the same message again and again
		void PTXCompilerWidget::render() {
			ImGui::Begin("PTX Compiler", 0, mWindowFlags);
			{
				ImGui::PushItemWidth(50);
				if (ImGui::Combo("Cuda Architecture##", &mSelCudaArch, mCudaArchs, 3)) {
					std::string plat = mPlatforms[mSelPlatform];
					std::string cuda = mCudaArchs[mSelCudaArch];
					comm::PTXCompilerMessage message(plat, "Debug", cuda, mRdc, mFastMath);
					mMessageFlowOut.get_subscriber().on_next(message);
				}
				ImGui::PopItemWidth();
				ImGui::PushItemWidth(50);
				if (ImGui::Combo("Platform##", &mSelPlatform, mPlatforms, 2)) {
					std::string plat = mPlatforms[mSelPlatform];
					std::string cuda = mCudaArchs[mSelCudaArch];
					comm::PTXCompilerMessage message(plat, "Debug", cuda, mRdc, mFastMath);
					mMessageFlowOut.get_subscriber().on_next(message);
				}
				ImGui::PopItemWidth();
				if (ImGui::Checkbox("Relocatable Device Code##", &mRdc)) {
					std::string plat = mPlatforms[mSelPlatform];
					std::string cuda = mCudaArchs[mSelCudaArch];
					comm::PTXCompilerMessage message(plat, "Debug", cuda, mRdc, mFastMath);
					mMessageFlowOut.get_subscriber().on_next(message);
				}
				if (ImGui::Checkbox("Use Fast Math##", &mFastMath)) {
					std::string plat = mPlatforms[mSelPlatform];
					std::string cuda = mCudaArchs[mSelCudaArch];
					comm::PTXCompilerMessage message(plat, "Debug", cuda, mRdc, mFastMath);
					mMessageFlowOut.get_subscriber().on_next(message);
				}
				if (ImGui::Button("Add Include Directory")) {
					std::string plat = mPlatforms[mSelPlatform];
					std::string cuda = mCudaArchs[mSelCudaArch];
					comm::PTXCompilerMessage message(plat, "Debug", cuda, mRdc, mFastMath);
					mMessageFlowOut.get_subscriber().on_next(message);
				}
			}
			ImGui::End();
			
		}

	}	//	!namespace widget
}	//	!namespace ui
#endif // !INCLUDE_EDITOR_UI_PTX_COMPILER_WIDGET_H_

