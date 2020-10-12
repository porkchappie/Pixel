#pragma once

#include <cinttypes>

#include "pxl/scancode.h"
#include "pxl/keycode.h"

namespace pxl {
	namespace key {
		enum struct action : uint8_t {
			down = 0, up = 1, repeat = 2
		};
	}
	namespace pointer {
		enum struct action : uint8_t {
			down = 0, up = 1, repeat = 2
		};
	}
	using namespace key;
	using namespace pointer;
}