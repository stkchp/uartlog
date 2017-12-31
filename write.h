// SPDX-License-Identifier: MIT

#ifndef UARTLOG_WRITE_H
#define UARTLOG_WRITE_H

#include <stdbool.h>
#include <stddef.h>

#define UARTLOG_MAXPATH 4096

struct write_context {
	bool renew;
	char *fmt;
	char *link;
	char filename[UARTLOG_MAXPATH];
	size_t length;
};

size_t dump_line(struct write_context *ctx, const char *buffer, size_t n);

#endif
