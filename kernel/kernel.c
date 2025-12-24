// kernel.c
// Very simple 32-bit kernel that writes text to the VGA text buffer.

#include <stdint.h>

// VGA text-mode buffer starts at this address
static uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;

// Screen dimensions in text mode
static const int VGA_WIDTH  = 80;
static const int VGA_HEIGHT = 25;

// Current cursor position
static int cursor_row = 0;
static int cursor_col = 0;

// Construct a VGA text-mode entry from a character and color attributes
static uint16_t vga_entry(char c, uint8_t fg, uint8_t bg) {
    uint8_t color = fg | (bg << 4);
    return (uint16_t)c | ((uint16_t)color << 8);
}

// Clear the screen by filling with spaces
static void clear_screen(uint8_t fg, uint8_t bg) {
    uint16_t blank = vga_entry(' ', fg, bg);
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            VGA_BUFFER[y * VGA_WIDTH + x] = blank;
        }
    }
    cursor_row = 0;
    cursor_col = 0;
}

// Put a single character at the current cursor position
static void putchar_color(char c, uint8_t fg, uint8_t bg) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else {
        VGA_BUFFER[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(c, fg, bg);
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= VGA_HEIGHT) {
        // For now, just reset to top if we overflow the screen
        cursor_row = 0;
    }
}

// Print a null-terminated string
static void print_color(const char* str, uint8_t fg, uint8_t bg) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        putchar_color(str[i], fg, bg);
    }
}

// Convenience: print with light gray on black
static void print(const char* str) {
    // 0x07 = light gray foreground, black background
    print_color(str, 0x07, 0x00);
}

// Entry point called from your assembly boot code
void kernel_main(void) {
    // Clear screen with black background, light gray text
    clear_screen(0x07, 0x00);

    print("Welcome to my OS kernel!\n");
    print("This text is written from kernel.c\n");
    print("\nYou can add more features from here...\n");

    // Hang the CPU in an infinite loop so it doesn't return
    while (1) {
        __asm__ __volatile__("hlt");
    }
}


