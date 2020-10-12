#pragma once

#include <cinttypes>

// entry point
namespace pxl {
	struct engine;
	struct application;
	struct init {
		const char* title;
		const uint32_t width, height;
		application* (*create_app)(engine*);
		init(const char* title, uint32_t width, uint32_t height, application* (*create_app)(engine*)) :
			title(title), width(width), height(height), create_app(create_app) {
		}
	};
}

extern pxl::init init_pxl();

#define PXL_INIT(TITLE, WIDTH, HEIGHT, APPLICATION) \
pxl::init init_pxl() { \
	return { TITLE, WIDTH, HEIGHT, [](pxl::engine* e) -> pxl::application* { \
		return new APPLICATION(e); \
	} }; \
} 
