#include "SUBLog.h"

void LOG_print(const char* log_type,
	const char* filename,
	const char* funcname,
	const int linenum,
	const char* time,
	const char* str, ...)
{
	va_list List;
	va_start(List,str);
	printf("%s | ", time);
	printf("%s | ", log_type);
	printf("FILE : %s / FUNC : %s / NUM : %d | ", filename, funcname, linenum);
	vprintf(str,List);
	va_end(List);
	printf("\n");
}