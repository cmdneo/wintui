#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "wintui/wintui.h"

int main()
{
	char buf[256] = { 0 };
	int event = 0;
	int run = 1;
	point2D pos = { 0 };

	surface *s = surface_init(1);
	if (NULL == s) {
		printf("Initialization failed!\n");
		return 1;
	}

	srand(time(NULL));
	wt_disable(TM_CURSOR);

	while (run) {
		int bytes = wt_read(buf, 256); /* LESS than 256 always */
		buf[bytes - 1] = '\0';
		if ('\x1b' == buf[0] && 1 == bytes)
			run = 0;

		surface_set(s, TC_TURQUOISE, TC_BLACK, TF_BOLD | TF_UNDERLINE);
		surface_draw(s, "X", pos.x, pos.y);

		event = surface_update(s);
		(void)(event = 2 * event);

		pos.x = 3 * (rand() % s->tsize.x / 3);
		pos.y = rand() % s->tsize.y;
	}

	/* INFO: As per relative sensetivity of colors to the human eye 
	 * Greyscale = 299 * r + .587 * g + .114 * b 
	 */

	surface_cleanup(s);
	return 0;
}
