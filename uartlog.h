// SPDX-License-Identifier: MIT

#ifndef UARTLOG_UARTLOG_H
#define UARTLOG_UARTLOG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct uartlog_options {
	bool version;
	bool usage;
	char *filefmt;
	char *needle;
	char *link;
	size_t nn;
	size_t buffsize;
};

#define UARTLOG_EOVER_BUFFER 1

#endif
