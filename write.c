// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "write.h"

static void clear_filename(struct write_context *ctx)
{
	memset(ctx->filename, '\0', TTYLOG_MAXPATH);
	ctx->length = 0;
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
		ctx->length = strftime(ctx->filename, TTYLOG_MAXPATH, ctx->fmt, tptr);
		if(ctx->length == 0) {
			clear_filename(ctx);
			return false;
		}
	}

	return true;
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

	wn = fwrite(buffer, sizeof(char), n, fp);
	
	fclose(fp);

	return wn;
}
