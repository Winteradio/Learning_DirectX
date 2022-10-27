#ifndef __LOG_H__
#define __LOG_H__

// Setting for see console Debug Log Message
#pragma comment(linker, "/entry:WinMainCRTStartup")
#pragma comment(linker, "/subsystem:console")

#include <stdio.h>
#include <stdarg.h>
#include <ctime>

// strrchr : Get Pointer for condition
// Get Pointer for address that is behind of '\\'
// if the address is C:\\File
// strrchr(__FILE__, '\\') -> true
// strrchr(__FILE__,'\\') + 1 -> File

// if the address is File
// strrchr(__FILE__, '\\') -> false
// __FILE__ -> File
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG_WARN(str, ...) LOG_print("WARN", __FILENAME__, __FUNCTION__, __LINE__, __TIME__, str, ##__VA_ARGS__)
#define LOG_ERROR(str, ...) LOG_print("ERROR", __FILENAME__, __FUNCTION__, __LINE__, __TIME__, str, ##__VA_ARGS__)
#define LOG_INFO(str, ...) LOG_print("INFO", __FILENAME__, __FUNCTION__, __LINE__, __TIME__, str, ##__VA_ARGS__)
#define LOG_DEBUG(str, ...) LOG_print("DEBUG", __FILENAME__, __FUNCTION__, __LINE__, __TIME__, str, ##__VA_ARGS__)
#define LOG_TRACE(str, ...) LOG_print("TRACE", __FILENAME__, __FUNCTION__, __LINE__, __TIME__, str, ##__VA_ARGS__)
#define LOG_ALL(str, ...) LOG_print("ALL", __FILENAME__, __FUNCTION__, __LINE__, __TIME__)printf(str, ##__VA_ARGS__, "\n")

void LOG_print(const char* log_type,
	const char* filename,
	const char* funcname,
	const int linenum,
	const char* time,
	const char* str, ...);

#endif