#ifndef INCLUDE_EDITOR_UI_POP_IP_H
#define INCLUDE_EDITOR_UI_POP_IP_H
namespace ui {
	namespace pop {
		template <typename... Args>
		class PopUp {
			virtual void activate() = 0;
			virtual void pop(const bool& pop, const Args& ...args) = 0;
		};
	}	//	!namespace pop
}	//	!namespace ui
#endif // !INCLUDE_EDITOR_UI_POP_IP_H

