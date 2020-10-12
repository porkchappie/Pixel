#pragma once

#include <vector>
#include <cassert>

#include "pxl/inputs.h"

namespace pxl {
	struct key_event {
		uint32_t timestamp;
		key::action action;
		keycode keycode;
		scancode scancode;
		modifier modifier;
	};
	struct pointer_event {
		uint32_t timestamp;
		pointer::action action;
		uint32_t which;
		uint8_t button;
		uint8_t clicks;
		modifier modifier;
		uint32_t x, y;

		//Uint32 type;        /**< ::SDL_MOUSEBUTTONDOWN or ::SDL_MOUSEBUTTONUP */
		//Uint32 timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
		//Uint32 windowID;    /**< The window with mouse focus, if any */
		//Uint32 which;       /**< The mouse instance id, or SDL_TOUCH_MOUSEID */
		//Uint8 button;       /**< The mouse button index */
		//Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
		//Uint8 clicks;       /**< 1 for single-click, 2 for double-click, etc. */
		//Uint8 padding1;
		//Sint32 x;           /**< X coordinate, relative to window */
		//Sint32 y;           /**< Y coordinate, relative to window */
	};

	template<typename T>
	struct event_listener;
	template<typename T>
	struct event_handler {
		static_assert(std::is_base_of_v<event_listener<T>, T>, "T is not base of event_listener");
		[[nodiscard]] inline auto begin() {
			return m_listeners.begin();
		}
		[[nodiscard]] inline auto begin() const {
			return m_listeners.begin();
		}
		[[nodiscard]] inline auto end() {
			return m_listeners.end();
		}
		[[nodiscard]] inline auto end() const {
			return m_listeners.end();
		}
		inline auto size() const { return m_listeners.size(); }

		inline void add(T* listener) {
			assert(listener && !listener->m_handler);
			listener->m_handler = this;
			listener->m_id = m_listeners.size();
			m_listeners.emplace_back(listener);
		}
		inline void remove(T* listener) {
			assert(listener && listener->m_handler == this);
			listener->m_handler = nullptr;
			const auto& back = m_listeners.back();
			m_listeners[back->m_id = listener->m_id] = back;
			m_listeners.pop_back();
		}
		inline auto operator[](const size_t& i) { return m_listeners[i]; }
		inline auto operator[](const size_t& i) const { return m_listeners[i]; }

	private:
		std::vector<T*> m_listeners;
	};
	template<typename T>
	struct event_listener {
		friend struct event_handler<T>;
	private:
		event_handler<T>* m_handler{ nullptr };
		size_t m_id{ 0 };
	};

	struct on_resize_listener : public event_listener<on_resize_listener> {
		virtual void on_resize(uint32_t width, uint32_t height) = 0;
	};

	struct on_key_listener : public event_listener<on_key_listener> {
		virtual void on_key(const key_event& e) = 0;
	};

	struct on_pointer_listener : public event_listener<on_pointer_listener> {
		virtual void on_pointer(const pointer_event& e) = 0;
	};
}