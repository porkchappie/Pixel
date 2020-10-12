#pragma once

#include <cinttypes>
#include <type_traits>

#include "pxl/events.h"

namespace pxl {
	struct engine;
	struct application {
		friend struct engine;
		struct event_handlers {
			event_handler<on_key_listener> key;
			event_handler<on_pointer_listener> pointer;
			event_handler<on_resize_listener> resize;
		} on;
		application(engine* engine) : m_engine{ engine } {}
	private:
		virtual void on_create() {}
		virtual void on_update(float dt) {}
		virtual void on_destroy() {}
		virtual void on_resize() {} // get window.
	private:
		engine* m_engine;
	};

}

#define PXL_APP(APPLICATION) \
	APPLICATION(pxl::engine* e) : pxl::application(e) {} 