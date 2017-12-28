// SPDX-License-Identifier: MIT

#ifndef TTYLOG_WRITE_H
#define TTYLOG_WRITE_H

#include <stdbool.h>
#include <stddef.h>

#define TTYLOG_MAXPATH 4096

struct write_context {
	bool renew;
	char *fmt;
	char filename[TTYLOG_MAXPATH];
	size_t length;
};

size_t dump_line(struct write_context *ctx, const char *buffer, size_t n);

#endif
