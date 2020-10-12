#include "pxl/scancode.h"

#include "SDL_keyboard.h"

namespace pxl {
	keycode to_keycode(scancode code) {
		return static_cast<keycode>(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(code)));
	}
}