// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <unistd.h> /* access/symlink */

#include <sys/stat.h> /* lstat */

#include "write.h"

static void clear_filename(struct write_context *ctx)
{
	memset(ctx->filename, '\0', UARTLOG_MAXPATH);
	ctx->length = 0;
}

static bool create_link(struct write_context *ctx)
{
	struct stat s;
	int r;

	/* check already link exists */
	r = stat(ctx->link, &s);
	if(r == 0)
	{
		if(remove(ctx->link) != 0)
		{
			return false;
		}
	}

	if(access(ctx->link, F_OK) == 0)
	{
		/* maybe other file exists */
		return false;
	}

	if(symlink(ctx->filename, ctx->link) != 0)
	{
		return false;
	}

	return true;
}

static bool renew_filename(struct write_context *ctx)
{
	if(ctx->length != 0)
	{
		clear_filename(ctx);
	}
	ctx->renew = false;

	{
		time_t t;
		struct tm *tptr;
		
		/* get current time */
		t = time(NULL);
		if(t == (time_t)(-1))
		{
			return false;
		}

		/* convert local time */
		tptr = localtime(&t);
		if(tptr == NULL)
		{
			return false;
		}

		/* time -> filename */
		ctx->length = strftime(ctx->filename, UARTLOG_MAXPATH, ctx->fmt, tptr);
		if(ctx->length == 0) {
			clear_filename(ctx);
			return false;
		}
	}

	if(ctx->link != NULL)
	{
		if(!create_link(ctx))
		{
			return false;
		}
	}

	return true;
}

#define TIMEFMT_SIZE 21
static inline size_t time_write(FILE *fp)
{

	time_t t;
	struct tm *tptr;
	char buffer[TIMEFMT_SIZE];
	size_t n;
	
	/* get current time */
	t = time(NULL);
	if(t == (time_t)(-1))
	{
		return false;
	}

	/* convert local time */
	tptr = localtime(&t);
	if(tptr == NULL)
	{
		return false;
	}

	/* time -> 21byte str */
	n = strftime(buffer, TIMEFMT_SIZE, "%Y/%m/%d %H:%M:%S ", tptr);
	if(n == TIMEFMT_SIZE - 1)
	{
		return fwrite(buffer, sizeof(char), sizeof(buffer) - 1, fp);
	}
	else
	{
		fprintf(stderr, "strftime %zu\n", n);
		return 0;
	}
}

size_t dump_line(struct write_context *ctx, const char *buffer, size_t n)
{
	size_t wn;
	FILE *fp = NULL;

	if(ctx->length == 0 || ctx->renew)
	{
		if(!renew_filename(ctx))
		{
			return 0;
		}
	}

	fp = fopen(ctx->filename, "a");
	if(fp == NULL)
	{
		return 0;
	}
	
	wn = time_write(fp);
	if(wn != 0)
	{
		wn = fwrite(buffer, sizeof(char), n, fp);
	}
	fclose(fp);

	return wn;
}
