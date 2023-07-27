#include <stdlib.h>

#include "fbi.h"

list lnew(size_t cap) {
	return (list) {
		malloc(sizeof(void *) * cap),
		0,
		cap
	};
}

void lappend(list *li, void *elem) {
	if (li->cap < 0)
		return;
	if (li->len == li->cap) {
		li->cap <<= 1;
		li->content = realloc(li->content, sizeof(void *) * li->cap);
	}
	++li->len;
	li->content[li->len] = elem;
}

void lfree(list *li) {
	free(li->content);
	li->len = 0;
	li->cap = -1;
}
