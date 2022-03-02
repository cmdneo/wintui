#ifndef INCLUDE_TERM_H
#define INCLUDE_TERM_H

typedef struct point2D {
	int x;
	int y;
} point2D;

typedef struct point2Df {
	double x;
	double y;
} point2Df;

typedef struct rgb {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb;

typedef struct pixel {
	rgb fg;
	rgb bg;
	point2D pos;
	char const *txt;
	unsigned int bold : 1;
	unsigned int dim : 1;
	unsigned int italic : 1;
	unsigned int underline : 1;
	unsigned int blink : 1;
	unsigned int inverse : 1;
	unsigned int invisible : 1;
	unsigned int strike : 1;
} pixel;

typedef struct surface {
	pixel *buf;
	pixel curfmt; /** Internal */
	point2D tsize;
	int bufcap;
	int ndeltas;
} surface;

/**
 * @brief Initialize by setting the terminal in raw mode
 * 
 * @param wait sets timeout for reading from stdin if no data is available.
 *             1 wait = 100 miliseconds.
 * @return surface* 
 */
surface *surface_init(int wait);

/**
 * @brief Reset the terminal back to the normal mode and cleanup
 * 
 * @param s 
 */
void surface_cleanup(surface *s);

void surface_set(surface *s, int fg_cc, int bg_cc, unsigned flags);
int surface_draw(surface *s, char const *txt, int x, int y);
int surface_update(surface *s);

/**
 * @brief Sets terminal to raw mode
 * 
 * @param minbytes min bytes to read from stdin, otherwise block for wait
 * @param wait if minbytes > 0 then timeout = 100 * wait miliseconds,
 * @return int 0 on success, otherwise -1
 */
int wt_set_raw(int minbytes, int wait);
int wt_restore();
void wt_clear();
void wt_move_cursor(int x, int y);
int wt_read(char *buf, unsigned n);
#define wt_print printf /* Swap as needed */

/**
 * @brief Modes/elements which an be either turned on or off.
 *        Use with wt_enable and wt_disable
 */
enum wt_mode {
	TM_CURSOR,
	TM_ALTBUF,
	TM_BKPASTE,
};
void wt_enable(enum wt_mode mode);
void wt_disable(enum wt_mode mode);

/**
 * @brief Actions, use with wt_do
 */
enum wt_action {
	TA_CLEAR_END,
	TA_CLEAT_START,
	TA_CLEAR,
	TA_CLEAR_LINE_START,
	TA_CLEAR_LINE_END,
	TA_CLEAR_LINE,
};
void wt_do(enum wt_action action);

/* DATA MEMBERS */

enum wt_pixel_flag {
	TF_BOLD = (1 << 0),
	TF_DIM = (1 << 1),
	TF_ITALIC = (1 << 2),
	TF_UNDERLINE = (1 << 3),
	TF_BLINK = (1 << 4),
	TF_INVERSE = (1 << 5),
	TF_INVISIBLE = (1 << 6),
	TF_STRIKE = (1 << 7),
};

enum wt_event {
	TE_QUIT = 1,
	TE_RESIZED,
};

enum wt_key {
	TK_UP,
	TK_DOWN,
	TK_LEFT,
	TK_RIGHT,
};

enum wt_color {
	TC_BISQUE = 0,
	TC_CORN_SILK,
	TC_SEA_SHELL,
	TC_TEAL,
	TC_PURPLE,
	TC_PALE_GREEN,
	TC_FIREBRICK,
	TC_MEDIUM_PURPLE,
	TC_PINK,
	TC_MEDIUM_BLUE,
	TC_MOCCASIN,
	TC_LIME,
	TC_PLUM,
	TC_LIGHT_GREY,
	TC_LIGHT_BLUE,
	TC_INDIAN_RED,
	TC_DARK_OLIVE_GREEN,
	TC_MEDIUM_VIOLET_RED,
	TC_PEACH_PUFF,
	TC_FOREST_GREEN,
	TC_DARK_GOLDEN_ROD,
	TC_DARK_SLATE_GRAY,
	TC_YELLOW,
	TC_SLATE_GRAY,
	TC_GOLD,
	TC_ROYAL_BLUE,
	TC_DARK_RED,
	TC_AQUA,
	TC_DARK_BLUE,
	TC_GREEN_YELLOW,
	TC_CHOCOLATE,
	TC_BROWN,
	TC_POWDER_BLUE,
	TC_DARK_MAGENTA,
	TC_MEDIUM_ORCHID,
	TC_LIGHT_SEA_GREEN,
	TC_LIGHT_SLATE_GRAY,
	TC_LAVENDER_BLUSH,
	TC_MEDIUM_SPRING_GREEN,
	TC_MAGENTA,
	TC_WHITE_SMOKE,
	TC_FLORAL_WHITE,
	TC_DARK_CYAN,
	TC_LIGHT_CYAN,
	TC_ORCHID,
	TC_GREY,
	TC_MEDIUM_TURQUOISE,
	TC_DARK_ORANGE,
	TC_DARK_SALMON,
	TC_DIM_GREY,
	TC_MIDNIGHT_BLUE,
	TC_VIOLET,
	TC_LIME_GREEN,
	TC_GREEN,
	TC_DARK_KHAKI,
	TC_MINT_CREAM,
	TC_DARK_SLATE_BLUE,
	TC_PAPAYA_WHIP,
	TC_CORN_FLOWER_BLUE,
	TC_DODGER_BLUE,
	TC_LEMON_CHIFFON,
	TC_GHOST_WHITE,
	TC_DEEP_PINK,
	TC_DARK_VIOLET,
	TC_THISTLE,
	TC_OLD_LACE,
	TC_MAROON,
	TC_STEEL_BLUE,
	TC_DARK_GRAY,
	TC_NAVAJO_WHITE,
	TC_OLIVE,
	TC_TAN,
	TC_SPRING_GREEN,
	TC_CORAL,
	TC_LAVENDER,
	TC_PALE_TURQUOISE,
	TC_SANDY_BROWN,
	TC_ROSY_BROWN,
	TC_SKY_BLUE,
	TC_LIGHT_STEEL_BLUE,
	TC_BLANCHED_ALMOND,
	TC_LIGHT_CORAL,
	TC_DEEP_SKY_BLUE,
	TC_LIGHT_YELLOW,
	TC_TURQUOISE,
	TC_SLATE_BLUE,
	TC_BLUE_VIOLET,
	TC_DARK_TURQUOISE,
	TC_SADDLE_BROWN,
	TC_OLIVE_DRAB,
	TC_PERU,
	TC_IVORY,
	TC_PALE_GOLDEN_ROD,
	TC_MEDIUM_AQUA_MARINE,
	TC_SILVER,
	TC_TOMATO,
	TC_HOT_PINK,
	TC_DARK_GREEN,
	TC_NAVY,
	TC_CRIMSON,
	TC_BLACK,
	TC_SNOW,
	TC_GAINSBORO,
	TC_AQUA_MARINE,
	TC_DARK_SEA_GREEN,
	TC_RED,
	TC_CHARTREUSE,
	TC_SIENNA,
	TC_BLUE,
	TC_LIGHT_GREEN,
	TC_LIGHT_PINK,
	TC_WHITE,
	TC_WHEAT,
	TC_DARK_GREY,
	TC_LIGHT_SALMON,
	TC_LIGHT_SKY_BLUE,
	TC_LIGHT_GOLDEN_ROD_YELLOW,
	TC_BURLY_WOOD,
	TC_ALICE_BLUE,
	TC_GRAY,
	TC_MEDIUM_SEA_GREEN,
	TC_ANTIQUE_WHITE,
	TC_MISTY_ROSE,
	TC_ORANGE_RED,
	TC_LINEN,
	TC_KHAKI,
	TC_LIGHT_GRAY,
	TC_YELLOW_GREEN,
	TC_CADET_BLUE,
	TC_SEA_GREEN,
	TC_DARK_ORCHID,
	TC_INDIGO,
	TC_DIM_GRAY,
	TC_CYAN,
	TC_AZURE,
	TC_LAWN_GREEN,
	TC_BEIGE,
	TC_ORANGE,
	TC_MEDIUM_SLATE_BLUE,
	TC_HONEYDEW,
	TC_PALE_VIOLET_RED,
	TC_SALMON,
	TC_GOLDEN_ROD,
	TC_COUNT, /* = Total colors */
};

/**
 * @brief Access by: T_PALLETE[TC_<name>]
 * 
 * Example: rgb bg_rgb_col = rgb[bg_cc];
 */
static rgb T_PALLETE[] = {
	[TC_BISQUE] = { 255, 228, 196 },
	[TC_CORN_SILK] = { 255, 248, 220 },
	[TC_SEA_SHELL] = { 255, 245, 238 },
	[TC_TEAL] = { 0, 128, 128 },
	[TC_PURPLE] = { 128, 0, 128 },
	[TC_PALE_GREEN] = { 152, 251, 152 },
	[TC_FIREBRICK] = { 178, 34, 34 },
	[TC_MEDIUM_PURPLE] = { 147, 112, 219 },
	[TC_PINK] = { 255, 192, 203 },
	[TC_MEDIUM_BLUE] = { 0, 0, 205 },
	[TC_MOCCASIN] = { 255, 228, 181 },
	[TC_LIME] = { 0, 255, 0 },
	[TC_PLUM] = { 221, 160, 221 },
	[TC_LIGHT_GREY] = { 211, 211, 211 },
	[TC_LIGHT_BLUE] = { 173, 216, 230 },
	[TC_INDIAN_RED] = { 205, 92, 92 },
	[TC_DARK_OLIVE_GREEN] = { 85, 107, 47 },
	[TC_MEDIUM_VIOLET_RED] = { 199, 21, 133 },
	[TC_PEACH_PUFF] = { 255, 218, 185 },
	[TC_FOREST_GREEN] = { 34, 139, 34 },
	[TC_DARK_GOLDEN_ROD] = { 184, 134, 11 },
	[TC_DARK_SLATE_GRAY] = { 47, 79, 79 },
	[TC_YELLOW] = { 255, 255, 0 },
	[TC_SLATE_GRAY] = { 112, 128, 144 },
	[TC_GOLD] = { 255, 215, 0 },
	[TC_ROYAL_BLUE] = { 65, 105, 225 },
	[TC_DARK_RED] = { 139, 0, 0 },
	[TC_AQUA] = { 0, 255, 255 },
	[TC_DARK_BLUE] = { 0, 0, 139 },
	[TC_GREEN_YELLOW] = { 173, 255, 47 },
	[TC_CHOCOLATE] = { 210, 105, 30 },
	[TC_BROWN] = { 165, 42, 42 },
	[TC_POWDER_BLUE] = { 176, 224, 230 },
	[TC_DARK_MAGENTA] = { 139, 0, 139 },
	[TC_MEDIUM_ORCHID] = { 186, 85, 211 },
	[TC_LIGHT_SEA_GREEN] = { 32, 178, 170 },
	[TC_LIGHT_SLATE_GRAY] = { 119, 136, 153 },
	[TC_LAVENDER_BLUSH] = { 255, 240, 245 },
	[TC_MEDIUM_SPRING_GREEN] = { 0, 250, 154 },
	[TC_MAGENTA] = { 255, 0, 255 },
	[TC_WHITE_SMOKE] = { 245, 245, 245 },
	[TC_FLORAL_WHITE] = { 255, 250, 240 },
	[TC_DARK_CYAN] = { 0, 139, 139 },
	[TC_LIGHT_CYAN] = { 224, 255, 255 },
	[TC_ORCHID] = { 218, 112, 214 },
	[TC_GREY] = { 128, 128, 128 },
	[TC_MEDIUM_TURQUOISE] = { 72, 209, 204 },
	[TC_DARK_ORANGE] = { 255, 140, 0 },
	[TC_DARK_SALMON] = { 233, 150, 122 },
	[TC_DIM_GREY] = { 105, 105, 105 },
	[TC_MIDNIGHT_BLUE] = { 25, 25, 112 },
	[TC_VIOLET] = { 238, 130, 238 },
	[TC_LIME_GREEN] = { 50, 205, 50 },
	[TC_GREEN] = { 0, 128, 0 },
	[TC_DARK_KHAKI] = { 189, 183, 107 },
	[TC_MINT_CREAM] = { 245, 255, 250 },
	[TC_DARK_SLATE_BLUE] = { 72, 61, 139 },
	[TC_PAPAYA_WHIP] = { 255, 239, 213 },
	[TC_CORN_FLOWER_BLUE] = { 100, 149, 237 },
	[TC_DODGER_BLUE] = { 30, 144, 255 },
	[TC_LEMON_CHIFFON] = { 255, 250, 205 },
	[TC_GHOST_WHITE] = { 248, 248, 255 },
	[TC_DEEP_PINK] = { 255, 20, 147 },
	[TC_DARK_VIOLET] = { 148, 0, 211 },
	[TC_THISTLE] = { 216, 191, 216 },
	[TC_OLD_LACE] = { 253, 245, 230 },
	[TC_MAROON] = { 128, 0, 0 },
	[TC_STEEL_BLUE] = { 70, 130, 180 },
	[TC_DARK_GRAY] = { 169, 169, 169 },
	[TC_NAVAJO_WHITE] = { 255, 222, 173 },
	[TC_OLIVE] = { 128, 128, 0 },
	[TC_TAN] = { 210, 180, 140 },
	[TC_SPRING_GREEN] = { 0, 255, 127 },
	[TC_CORAL] = { 255, 127, 80 },
	[TC_LAVENDER] = { 230, 230, 250 },
	[TC_PALE_TURQUOISE] = { 175, 238, 238 },
	[TC_SANDY_BROWN] = { 244, 164, 96 },
	[TC_ROSY_BROWN] = { 188, 143, 143 },
	[TC_SKY_BLUE] = { 135, 206, 235 },
	[TC_LIGHT_STEEL_BLUE] = { 176, 196, 222 },
	[TC_BLANCHED_ALMOND] = { 255, 235, 205 },
	[TC_LIGHT_CORAL] = { 240, 128, 128 },
	[TC_DEEP_SKY_BLUE] = { 0, 191, 255 },
	[TC_LIGHT_YELLOW] = { 255, 255, 224 },
	[TC_TURQUOISE] = { 64, 224, 208 },
	[TC_SLATE_BLUE] = { 106, 90, 205 },
	[TC_BLUE_VIOLET] = { 138, 43, 226 },
	[TC_DARK_TURQUOISE] = { 0, 206, 209 },
	[TC_SADDLE_BROWN] = { 139, 69, 19 },
	[TC_OLIVE_DRAB] = { 107, 142, 35 },
	[TC_PERU] = { 205, 133, 63 },
	[TC_IVORY] = { 255, 255, 240 },
	[TC_PALE_GOLDEN_ROD] = { 238, 232, 170 },
	[TC_MEDIUM_AQUA_MARINE] = { 102, 205, 170 },
	[TC_SILVER] = { 192, 192, 192 },
	[TC_TOMATO] = { 255, 99, 71 },
	[TC_HOT_PINK] = { 255, 105, 180 },
	[TC_DARK_GREEN] = { 0, 100, 0 },
	[TC_NAVY] = { 0, 0, 128 },
	[TC_CRIMSON] = { 220, 20, 60 },
	[TC_BLACK] = { 0, 0, 0 },
	[TC_SNOW] = { 255, 250, 250 },
	[TC_GAINSBORO] = { 220, 220, 220 },
	[TC_AQUA_MARINE] = { 127, 255, 212 },
	[TC_DARK_SEA_GREEN] = { 143, 188, 143 },
	[TC_RED] = { 255, 0, 0 },
	[TC_CHARTREUSE] = { 127, 255, 0 },
	[TC_SIENNA] = { 160, 82, 45 },
	[TC_BLUE] = { 0, 0, 255 },
	[TC_LIGHT_GREEN] = { 144, 238, 144 },
	[TC_LIGHT_PINK] = { 255, 182, 193 },
	[TC_WHITE] = { 255, 255, 255 },
	[TC_WHEAT] = { 245, 222, 179 },
	[TC_DARK_GREY] = { 169, 169, 169 },
	[TC_LIGHT_SALMON] = { 255, 160, 122 },
	[TC_LIGHT_SKY_BLUE] = { 135, 206, 250 },
	[TC_LIGHT_GOLDEN_ROD_YELLOW] = { 250, 250, 210 },
	[TC_BURLY_WOOD] = { 222, 184, 135 },
	[TC_ALICE_BLUE] = { 240, 248, 255 },
	[TC_GRAY] = { 128, 128, 128 },
	[TC_MEDIUM_SEA_GREEN] = { 60, 179, 113 },
	[TC_ANTIQUE_WHITE] = { 250, 235, 215 },
	[TC_MISTY_ROSE] = { 255, 228, 225 },
	[TC_ORANGE_RED] = { 255, 69, 0 },
	[TC_LINEN] = { 250, 240, 230 },
	[TC_KHAKI] = { 240, 230, 140 },
	[TC_LIGHT_GRAY] = { 211, 211, 211 },
	[TC_YELLOW_GREEN] = { 154, 205, 50 },
	[TC_CADET_BLUE] = { 95, 158, 160 },
	[TC_SEA_GREEN] = { 46, 139, 87 },
	[TC_DARK_ORCHID] = { 153, 50, 204 },
	[TC_INDIGO] = { 75, 0, 130 },
	[TC_DIM_GRAY] = { 105, 105, 105 },
	[TC_CYAN] = { 0, 255, 255 },
	[TC_AZURE] = { 240, 255, 255 },
	[TC_LAWN_GREEN] = { 124, 252, 0 },
	[TC_BEIGE] = { 245, 245, 220 },
	[TC_ORANGE] = { 255, 165, 0 },
	[TC_MEDIUM_SLATE_BLUE] = { 123, 104, 238 },
	[TC_HONEYDEW] = { 240, 255, 240 },
	[TC_PALE_VIOLET_RED] = { 219, 112, 147 },
	[TC_SALMON] = { 250, 128, 114 },
	[TC_GOLDEN_ROD] = { 218, 165, 32 },
};

#endif
