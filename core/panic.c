#include "panic_config.h"

// Pointer to system's raw VRAM graphics frame buffer address
extern uint32_t* system_frame_buffer; 

// A tiny, built-in 8x8 font bitmap stub for drawing characters raw
// (Each byte represents 1 row of 8 horizontal pixels)
extern const uint8_t basic_font_8x8[128][8];

// Internal helper to draw a simple solid color block for the window background
void draw_solid_box(int start_x, int start_y, int width, int height, uint32_t color) {
    for (int y = start_y; y < (start_y + height); y++) {
        for (int x = start_x; x < (start_x + width); x++) {
            // Raw linear memory calculation: (Y * Pitch) + X
            system_frame_buffer[y * SCREEN_WIDTH + x] = color;
        }
    }
}

// Internal helper to render an ASCII character using your 8x8 bitmap array
void draw_char(char c, int start_x, int start_y, uint32_t color) {
    for (int row = 0; row < 8; row++) {
        uint8_t row_bits = basic_font_8x8[(uint8_t)c][row];
        for (int col = 0; col < 8; col++) {
            // Test if the bit is active from left to right (0x80 down to 0x01)
            if (row_bits & (0x80 >> col)) {
                system_frame_buffer[(start_y + row) * SCREEN_WIDTH + (start_x + col)] = color;
            }
        }
    }
}

// Simple string rendering function that handles X coordinate stepping
void draw_string(const char* str, int x, int y, uint32_t color) {
    int current_x = x;
    while (*str) {
        draw_char(*str, current_x, y, color);
        current_x += 8; // Step 8 pixels forward to the next character
        str++;
    }
}

// Core execution path called by Ring 0 when the mount sequence fails
void draw_graphics_kernel_panic(const char* system_fault_reason) {
    // 1. Freeze the existing graphical display by drawing a solid black window overlay
    draw_solid_box(BOX_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, COLOR_BLACK);
    
    // 2. Draw a striking red accent border around the black square window
    // (Top line, bottom line, left wall, right wall offsets)
    draw_solid_box(BOX_X, BOX_Y, BOX_WIDTH, 2, COLOR_RED);
    draw_solid_box(BOX_X, BOX_Y + BOX_HEIGHT - 2, BOX_WIDTH, 2, COLOR_RED);
    draw_solid_box(BOX_X, BOX_Y, 2, BOX_HEIGHT, COLOR_RED);
    draw_solid_box(BOX_X + BOX_WIDTH - 2, BOX_Y, 2, BOX_HEIGHT, COLOR_RED);

    // 3. Render the structural warning strings into the black box area
    draw_string("KERNEL PANIC!", BOX_X + 20, BOX_Y + 20, COLOR_RED);
    draw_string("=======================================", BOX_X + 20, BOX_Y + 35, COLOR_WHITE);
    
    draw_string("Reason:", BOX_X + 20, BOX_Y + 55, COLOR_WHITE);
    draw_string(system_fault_reason, BOX_X + 30, BOX_Y + 75, COLOR_WHITE);
    
    draw_string("init died, can't find unknownblock0.0", BOX_X + 20, BOX_Y + 105, COLOR_WHITE);
    draw_string("System execution halted permanently.", BOX_X + 20, BOX_Y + 135, COLOR_RED);

    // 4. Force the CPU out of processing tasks completely
    asm volatile (
        "cli\n\t"
        "1:\n\t"
        "hlt\n\t"
        "jmp 1b"
    );
}
