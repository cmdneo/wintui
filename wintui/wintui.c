#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "wintui/wintui.h"

/* Internal globals */

static struct termios g_old_termios;
static struct winsize g_wsize;

enum { INIT_BUF_CAP = 256 };

int wt_set_raw(int minbytes, int wait)
{
	struct termios t;
	if (0 != tcgetattr(STDOUT_FILENO, &t))
		return -1;

	cfmakeraw(&t);
	t.c_cc[VTIME] = wait;
	t.c_cc[VMIN] = minbytes;

	if (0 != tcsetattr(STDOUT_FILENO, TCSADRAIN, &t))
		return -1;
	return 0;
}

int wt_restore()
{
	if (0 != tcsetattr(STDIN_FILENO, TCSADRAIN, &g_old_termios)) {
		fprintf(stderr,
			"ERROR: Failed to reset terminal back to normal mode!\n");
		return -1;
	}
	return 0;
}

void wt_clear()
{
	wt_print("\x1b[0m\x1b[2J");
	fflush(stdout);
}

void wt_do(enum wt_action action)
{
	printf("%d", action);
}

void wt_enable(enum wt_mode mode)
{
	char *s;
	switch (mode) {
	case TM_CURSOR:
		s = "\x1b[?25h";
		break;
	case TM_ALTBUF:
		s = "\x1b[?1049h";
		break;
	case TM_BKPASTE:
		s = "\x1b[?2004h";
		break;
	};
	wt_print("%s", s);
}

void wt_disable(enum wt_mode mode)
{
	char *s;
	switch (mode) {
	case TM_CURSOR:
		s = "\x1b[?25l";
		break;
	case TM_ALTBUF:
		s = "\x1b[?1049l";
		break;
	case TM_BKPASTE:
		s = "\x1b[?2004l";
		break;
	};
	wt_print("%s", s);
}

void wt_move_cursor(int x, int y)
{
	/* Terminal coords are as line,col and start from 1 */
	wt_print("\x1b[%d;%dH", y + 1, x + 1);
}

int wt_read(char *buf, unsigned n)
{
	return read(STDIN_FILENO, buf, n);
}

static inline int wt_clamp(int v, int low, int high)
{
	if (v > high)
		return high;
	if (v < low)
		return low;
	return v;
}

static void pixel_write(pixel *p)
{
	int x = p->pos.x;
	int y = p->pos.y;
	wt_move_cursor(x, y);
	wt_print("\x1b[38;2;%hhu;%hhu;%hhum\x1b[48;2;%hhu;%hhu;%hhum", p->fg.r,
		 p->fg.g, p->fg.b, p->bg.r, p->bg.g, p->bg.b);
	if (p->bold)
		wt_print("\x1b[1m");
	if (p->dim)
		wt_print("\x1b[2m");
	if (p->italic)
		wt_print("\x1b[3m");
	if (p->underline)
		wt_print("\x1b[4m");
	if (p->blink)
		wt_print("\x1b[5m");
	if (p->inverse)
		wt_print("\x1b[7m");
	if (p->invisible)
		wt_print("\x1b[8m");
	if (p->strike)
		wt_print("\x1b[9m");
	wt_print("%s", p->txt);
}

/* SURFACE FUNCTIONS */

static int surface_resize_buffer(surface *s)
{
	pixel *buf = realloc(s->buf, s->bufcap * (sizeof *buf));

	if (NULL == buf) {
		return -1;
	} else {
		s->buf = buf;
		return 0;
	}
}

void surface_set(surface *s, rgb fg, rgb bg, unsigned flags)
{
	s->curfmt = (pixel){
		.fg = fg,
		.bg = bg,
		.bold = !!(flags & TPF_BOLD),
		.dim = !!(flags & TPF_DIM),
		.italic = !!(flags & TPF_ITALIC),
		.underline = !!(flags & TPF_UNDERLINE),
		.blink = !!(flags & TPF_BLINK),
		.inverse = !!(flags & TPF_INVERSE),
		.invisible = !!(flags & TPF_INVISIBLE),
		.strike = !!(flags & TPF_STRIKE),
	};
}

int surface_draw(surface *s, char const *txt, int x, int y)
{
	x = wt_clamp(x, 0, s->tsize.x - 1);
	y = wt_clamp(y, 0, s->tsize.y - 1);
	s->buf[s->ndeltas] = s->curfmt;
	s->buf[s->ndeltas].txt = txt;
	s->buf[s->ndeltas].pos = (point2D){ x, y };
	s->ndeltas++;

	if (s->bufcap <= s->ndeltas) {
		s->bufcap *= 2;
		if (0 != surface_resize_buffer(s))
			return -1;
		return 0;
	}
	return 0;
}

surface *surface_init(int wait)
{
	surface *ret = NULL;
	pixel *buf = NULL;
	point2D tsize = { 0 };

	if (0 != ioctl(STDOUT_FILENO, TIOCGWINSZ, &g_wsize)) {
		fprintf(stderr, "Error: IOCTL call failed!\n");
		goto ERR;
	} else {
		tsize.x = g_wsize.ws_col;
		tsize.y = g_wsize.ws_row;
	}
	/* Roll back each step after error then and goto and return */
	if (0 != tcgetattr(STDIN_FILENO, &g_old_termios)) {
		fprintf(stderr, "Error: Termios failed!\n");
		goto ERR;
	} else if (0 != wt_set_raw(0, wait)) {
		fprintf(stderr, "Error: Termios failed!\n");
		goto ERR;
	} else if (NULL == (ret = malloc(sizeof *ret))) {
		wt_restore();
		goto MEM_ERR;
	} else if (NULL == (buf = calloc(INIT_BUF_CAP, sizeof *buf))) {
		wt_restore();
		free(ret);
		ret = NULL;
		goto MEM_ERR;
	}

	*ret = (surface){
		.bufcap = INIT_BUF_CAP,
		.tsize = tsize,
		.buf = buf,
	};

	wt_enable(TM_ALTBUF);
	wt_clear();

	return ret;

MEM_ERR:
	fprintf(stderr, "Error: Memory allocation error!\n");
ERR: /* nothing */
	return NULL;
}

int surface_update(surface *s)
{
	/* Update terminal size */
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &g_wsize);

	/*
	 * Do not draw when the terminal size changes 
	 * just clear the screen and reset text attrs
	 */
	if (s->tsize.x != g_wsize.ws_col || s->tsize.y != g_wsize.ws_row) {
		s->tsize.x = g_wsize.ws_col;
		s->tsize.y = g_wsize.ws_row;
		s->ndeltas = 0;
		wt_clear();
		return TE_RESIZED;
	}

	for (int i = 0; i < s->ndeltas; i++)
		pixel_write(&(s->buf[i]));
	s->ndeltas = 0;
	fflush(stdout);

	return 0;
}

void surface_cleanup(surface *s)
{
	wt_clear();
	wt_restore();
	wt_disable(TM_ALTBUF);
	wt_enable(TM_CURSOR);

	free(s->buf);
	free(s);
}
