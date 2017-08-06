#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

internal void die_with_user_msg(char *message, const char *detail)
{
	fputs(message, stderr);
	fputs(": ", stderr);
	fputs(detail, stderr);
	fputc('\n', stderr);
	exit(1);
}

internal void die_with_sys_msg(const char *message)
{
	perror(message);
	exit(1);
}
