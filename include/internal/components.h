#ifndef INCLUDE_INTERNAL_COMPONENTS_H
#define INCLUDE_INTERNAL_COMPONENTS_H

typedef struct textfield {
} textfield;

typedef struct window {
} window;

typedef struct dialog_cnf {
	char *btn_yes_txt;
	char *btn_no_txt;
} dialog_cnf;

typedef struct dialog_info {
	char *btn_txt;
} dialog_info;

typedef struct component {
	union {
	};
	int type;
	void (*callback)(component *cpm, int event, void *arg);
} component;

#endif