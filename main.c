#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "wintui/wintui.h"

int main()
{
	// Testing...
	char buf[256] = { 0 };
	int event = 0;
	bool run = true;

	int fg_col = TC_RED;
	int rows = 0;

	surface *s = surface_init(1);
	if (NULL == s) {
		printf("Initialization failed!\n");
		return 1;
	}

	srand(time(NULL));
	wt_disable(TM_CURSOR);
	wt_clear();

	while (run) {
		// Handle ESC for quit
		int bytes = wt_read(buf, 255);
		buf[bytes] = '\0';
		if ('\x1b' == buf[0] && 1 == bytes)
			run = false;

		surface_set(s, T_PALLETE[fg_col], T_PALLETE[TC_BLACK],
			    TPF_BOLD);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j <= i; j++)
				surface_draw(s, "* ", j * 2, i);
		}

		event = surface_update(s);

		if (++rows > 12) {
			wt_clear();
			fg_col++;
			fg_col %= TC_COUNT;
			rows = 0;
		}
		/* INFO: As per relative sensetivity of colors to the human eye 
	 * Greyscale = 299 * r + .587 * g + .114 * b 
	 */
	}

	surface_cleanup(s);
	return 0;
}
