#ifndef PANIC_CONFIG_H
#define PANIC_CONFIG_H

#include <stdint.h>

// Explicit dimensions based on your OS graphics video mode configuration
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// Panic box properties (Box coordinates centered on screen)
#define BOX_WIDTH     450
#define BOX_HEIGHT    180
#define BOX_X         ((SCREEN_WIDTH - BOX_WIDTH) / 2)
#define BOX_Y         ((SCREEN_HEIGHT - BOX_HEIGHT) / 2)

// RGB Color Schemes (Assuming 32-bit color depth / 4 bytes per pixel)
#define COLOR_BLACK   0x00000000
#define COLOR_WHITE   0x00FFFFFF
#define COLOR_RED     0x00FF0000

#endif
