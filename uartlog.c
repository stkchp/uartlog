// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include "uartlog.h"
#include "read.h"

#ifndef UARTLOG_VERSION
#define UARTLOG_VERSION "0.0.1"
#endif

struct uartlog_options opts = {
	.usage   = false,
	.version = false,
	.filefmt = NULL,
	.needle = NULL,
	.link = NULL,
	.nn = 0,
	.buffsize = 1024
};

static void show_usage(char *name)
{
	char *base = name;
	while(name[0] != '\0')
	{
		if(name[0] == '/')
		{
			base = name + 1;
		}
		name++;
	}
	printf("usage: %s -s search -f format\n", base);
}

static void show_version(void)
{
	printf(" uartlog version: " UARTLOG_VERSION "\n");
	printf(" build date: " __TIME__ ", " __DATE__ "\n");
}

static bool parse_args(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"search", required_argument, NULL, 's'},
		{"format", required_argument, NULL, 'f'},
		{"link", required_argument, NULL, 'l'},
		{"version", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	while(1)
	{
		int c;
		int r = 0;
		int optidx = 0;

		/* getopt no error message. */
		opterr = 0;

		c = getopt_long(argc, argv, "vhs:f:l:", long_options, &optidx);

		if(c < 0)
		{
			break;
		}

		switch(c)
		{
		case 'v':
			opts.version = true;
			break;
		case 'h':
			opts.usage = true;
			break;
		case 'l':
			opts.link = optarg;
			break;
		case 'f':
			opts.filefmt = optarg;
			break;
		case 's':
			opts.needle = optarg;
			opts.nn = strlen(opts.needle);
			break;
		default:
			fprintf(stderr, "Unrecognized option(s)\n");
			r = -1;
			break;
		}

		if(r < 0)
		{
			fprintf(stderr, "run with '--help'.\n");
			return false;
		}
	}

	return true;
}

static bool check_options(void)
{
	/* check options */
	if(opts.filefmt == NULL)
	{
		fprintf(stderr, "'--format' option must be specified.\n");
		return false;
	}
	if(opts.needle == NULL)
	{
		fprintf(stderr, "'--search' option must be specified.\n");
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{
	if(!parse_args(argc, argv))
	{
		return 1;
	}

	if(opts.usage)
	{
		show_usage(argv[0]);
		return 0;
	}

	if(opts.version)
	{
		show_version();
		return 0;
	}

	if(!check_options())
	{
		return 1;
	}
	
	read_stdin(&opts);

	return 0;
}
