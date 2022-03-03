#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "wintui/wintui.h"

int main()
{
	char buf[256] = { 0 };
	int event = 0;
	int run = 1;
	int fg_col = 1;
	point2D pos = { .x = 40, .y = 20 };

	surface *s = surface_init(1);
	if (NULL == s) {
		printf("Initialization failed!\n");
		return 1;
	}

	srand(time(NULL));
	wt_disable(TM_CURSOR);

	while (run) {
		int bytes = wt_read(buf, 255); /* LESS than 256 always */
		buf[bytes] = '\0';
		if ('\x1b' == buf[0] && 1 == bytes)
			run = 0;

		surface_set(s, fg_col, TC_BLACK, TF_BOLD | TF_UNDERLINE);
		for (int i = 0; i < 50; i++) {
			surface_draw(
				s, "*",
				2 * pos.x + 2 * (int)(10 * sin(2 * 3.14 *
							       (i / 50.))),
				pos.y + (int)(10 * cos(2 * 3.14 * (i / 50.))));
		}

		event = surface_update(s);
		fg_col++;
		fg_col %= TC_COUNT;
	}

	/* INFO: As per relative sensetivity of colors to the human eye 
	 * Greyscale = 299 * r + .587 * g + .114 * b 
	 */

	surface_cleanup(s);
	return 0;
}
