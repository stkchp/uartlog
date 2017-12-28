// SPDX-License-Identifier: MIT

#ifndef TTYLOG_TTYLOG_H
#define TTYLOG_TTYLOG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct ttylog_options {
	bool version;
	bool usage;
	char *filefmt;
	char *needle;
	char *link;
	size_t nn;
	size_t buffsize;
};

#define TTYLOG_EOVER_BUFFER 1

#endif
