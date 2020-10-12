#include "pxl/keycode.h"

#include "SDL_keyboard.h"

namespace pxl {
	scancode to_scancode(keycode code) {
		return static_cast<scancode>(SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(code)));
	}
}