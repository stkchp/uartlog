// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "write.h"

#include "read.h"

static bool search_string(char *haystack, size_t hn, char *needle, size_t nn)
{
	if(nn > hn)
		return false;

	if(strstr(haystack, needle) == NULL)
		return false;

	return true;
}

int read_stdin(struct uartlog_options *opts)
{
	char *buffer = calloc(opts->buffsize, sizeof(char));

	size_t i = 0;
	int c;
	int rc = 0;
	struct write_context ctx = {0};
	ctx.fmt = opts->filefmt;
	ctx.link = opts->link;

	while((c = fgetc(stdin)) != EOF)
	{
		buffer[i] = (c & 0xff);
		if(buffer[i] == '\n')
		{
			ctx.renew = search_string(buffer, i + 1, opts->needle, opts->nn);
			dump_line(&ctx, buffer, i + 1);
			i = 0;
			memset(buffer, '\0', opts->buffsize);
		}
		else 
		{
			i++;
			if(i >= opts->buffsize)
			{
				rc = UARTLOG_EOVER_BUFFER;
				goto end;
			}
		}
	}

	/* dump remain buffer. */
	if(i != 0)
	{
		buffer[i] = '\n';
		dump_line(&ctx, buffer, i + 1);
	}

end:
	free(buffer);
	return 0;
}
