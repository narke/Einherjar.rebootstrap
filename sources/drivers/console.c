#include <drivers/console.h>
#include <lib/typedefs.h>
#include <openfirmware/ofw.h>
#include "font8x8_basic.h"

#define GLYPH_W  8
#define GLYPH_H  8

#define COLOR_R_YELLOW  255
#define COLOR_G_YELLOW  255
#define COLOR_B_YELLOW  0
#define COLOR_R_BLUE    0
#define COLOR_G_BLUE    0
#define COLOR_B_BLUE    255

/* OFW/VGA-like palette indexes for depth <= 8 */
#define PALETTE_INDEX_BLUE    1
#define PALETTE_INDEX_YELLOW  14

typedef struct {
	uint8_t *base;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	uint32_t pitch;
	uint32_t fg;
	uint32_t bg;
	uint32_t cols;
	uint32_t rows;
	uint32_t cx;
	uint32_t cy;
	bool ready;
} fb_console_t;

static fb_console_t fb;

static uint32_t cursor_px(void)
{
	return fb.cx * GLYPH_W;
}

static uint32_t cursor_py(void)
{
	return fb.cy * GLYPH_H;
}

static uint32_t color_to_pixel(uint8_t r, uint8_t g, uint8_t b)
{
	if (fb.depth <= 8) {
		if (r == COLOR_R_YELLOW && g == COLOR_G_YELLOW && b == COLOR_B_YELLOW)
			return PALETTE_INDEX_YELLOW;
		return PALETTE_INDEX_BLUE;
	}

	if (fb.depth == 15)
		return ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3);

	if (fb.depth == 16)
		return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);

	/* 24/32-bpp, write as 0x00RRGGBB */
	return (r << 16) | (g << 8) | b;
}

static void put_pixel(uint32_t x, uint32_t y, uint32_t pix)
{
	uint8_t *p;

	if (!fb.ready || x >= fb.width || y >= fb.height)
		return;

	p = fb.base + (y * fb.pitch);

	if (fb.depth <= 8) {
		p[x] = (uint8_t)pix;
		return;
	}

	if (fb.depth == 15 || fb.depth == 16) {
		uint16_t *p16 = (uint16_t *)p;
		p16[x] = (uint16_t)pix;
		return;
	}

	if (fb.depth == 24) {
		uint8_t *d = p + x * 3;
		d[0] = (uint8_t)(pix & 0xff);
		d[1] = (uint8_t)((pix >> 8) & 0xff);
		d[2] = (uint8_t)((pix >> 16) & 0xff);
		return;
	}

	((uint32_t *)p)[x] = pix;
}

static void fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
	uint32_t yy, xx;

	for (yy = 0; yy < h; yy++) {
		for (xx = 0; xx < w; xx++)
			put_pixel(x + xx, y + yy, color);
	}
}

static void draw_glyph(char c, uint32_t px, uint32_t py)
{
	uint8_t ch = (uint8_t)c;
	uint32_t y, x;

	if (ch > 127)
		ch = '?';

	for (y = 0; y < GLYPH_H; y++) {
		uint8_t row = font8x8_basic[ch][y];
		for (x = 0; x < GLYPH_W; x++) {
			uint32_t color = (row & (1 << x)) ? fb.fg : fb.bg;
			put_pixel(px + x, py + y, color);
		}
	}
}

static void scroll_up(void)
{
	uint32_t y, x;
	uint32_t text_h = fb.rows * GLYPH_H;

	if (text_h <= GLYPH_H)
		return;

	for (y = GLYPH_H; y < text_h; y++) {
		uint8_t *src = fb.base + y * fb.pitch;
		uint8_t *dst = fb.base + (y - GLYPH_H) * fb.pitch;
		for (x = 0; x < fb.pitch; x++)
			dst[x] = src[x];
	}

	fill_rect(0, text_h - GLYPH_H, fb.width, GLYPH_H, fb.bg);
}

static void newline(void)
{
	fb.cx = 0;
	fb.cy++;
	if (fb.cy >= fb.rows) {
		scroll_up();
		fb.cy = fb.rows - 1;
	}
}

static bool fb_read_geometry(phandle pkg, uint32_t *width, uint32_t *height,
    uint32_t *depth, uint32_t *linebytes)
{
	if ((int)ofw_get_property(pkg, "width", width, sizeof(*width)) <= 0)
		return false;
	if ((int)ofw_get_property(pkg, "height", height, sizeof(*height)) <= 0)
		return false;
	if ((int)ofw_get_property(pkg, "depth", depth, sizeof(*depth)) <= 0)
		return false;
	if ((int)ofw_get_property(pkg, "linebytes", linebytes, sizeof(*linebytes)) <= 0)
		return false;

	return true;
}

static void fb_configure(uint32_t addr, uint32_t width, uint32_t height,
    uint32_t depth, uint32_t linebytes)
{
	fb.base = (uint8_t *)(uintptr_t)addr;
	fb.width = width;
	fb.height = height;
	fb.depth = depth;
	fb.pitch = linebytes;
	fb.cols = width / GLYPH_W;
	fb.rows = height / GLYPH_H;
	fb.cx = 0;
	fb.cy = 0;
	fb.fg = color_to_pixel(COLOR_R_YELLOW, COLOR_G_YELLOW, COLOR_B_YELLOW);
	fb.bg = color_to_pixel(COLOR_R_BLUE, COLOR_G_BLUE, COLOR_B_BLUE);
	fb.ready = (fb.cols > 0 && fb.rows > 0);
}

static bool fb_detect(void)
{
	phandle pkg;
	ofw_arg_t rets[2];
	uint32_t addr = 0;
	uint32_t width = 0, height = 0, depth = 0, linebytes = 0;

	if (ofw_stdout == 0)
		return false;

	pkg = (phandle)ofw_call("instance-to-package", 1, 1, NULL, ofw_stdout);
	if (pkg == (phandle)-1 || pkg == 0)
		return false;

	if (!fb_read_geometry(pkg, &width, &height, &depth, &linebytes))
		return false;

	/* Preferred: OFW method returning mapped framebuffer address. */
	if ((int)ofw_call("call-method", 2, 3, rets, "frame-buffer-adr", ofw_stdout) == 0)
		addr = (uint32_t)rets[0];

	/* Fallback: "address" property. */
	if (addr == 0)
		(void)ofw_get_property(pkg, "address", &addr, sizeof(addr));

	if (addr == 0 || width == 0 || height == 0 || linebytes == 0)
		return false;

	fb_configure(addr, width, height, depth, linebytes);
	return fb.ready;
}

static void fallback_putchar(char c)
{
	ofw_putchar(c);
}

static void fallback_write(const char *s)
{
	while (*s)
		fallback_putchar(*s++);
}

static void fb_backspace(void)
{
	if (fb.cx > 0)
		fb.cx--;
	draw_glyph(' ', cursor_px(), cursor_py());
}

void console_clear(void)
{
	if (!fb.ready) {
		fallback_write("\n");
		return;
	}
	fill_rect(0, 0, fb.width, fb.height, fb.bg);
	fb.cx = 0;
	fb.cy = 0;
}

void console_init(void)
{
	fb.ready = false;
	if (fb_detect())
		console_clear();
}

void console_puts(const char *s)
{
	if (!fb.ready) {
		fallback_write(s);
		return;
	}
	while (*s)
		console_putchar(*s++);
}

void console_putchar(char c)
{
	if (!fb.ready) {
		fallback_putchar(c);
		return;
	}

	switch (c) {
	case '\n':
		newline();
		return;
	case '\r':
		fb.cx = 0;
		return;
	case '\b':
		fb_backspace();
		return;
	default:
		break;
	}

	draw_glyph(c, cursor_px(), cursor_py());
	fb.cx++;
	if (fb.cx >= fb.cols)
		newline();
}
