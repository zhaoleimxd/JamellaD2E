// CommandLine.cpp
#include "JamellaD2E.h"
static char ProgramFilePathBuffer[260];
static char InitialLoadFileBuffer[260];
void ParseCommandLine()
{
	WCHAR* cl = GetCommandLineW();
	char BufferA[1024];
	WideCharToMultiByte(CP_ACP, 0, cl, -1, BufferA, sizeof BufferA, NULL, NULL);
	char* s = BufferA;
	char* d = ProgramFilePathBuffer;
	int QuoteDepth = 0;
	int cln = 0;
	while (*s != 0)
	{
		if (*s == '"' && QuoteDepth <= 0)
		{
			QuoteDepth++;
			s++;
		}
		else if (*s == '"' && QuoteDepth > 0)
		{
			QuoteDepth--;
			s++;
		}
		else if (*s == ' ' && QuoteDepth <= 0)
		{
			if (d)
				*d = 0;
			cln++;
			if (cln == 1)
				d = InitialLoadFileBuffer;
			else
				d = 0;
			s++;
		}
		else
		{
			if (d)
				*d++ = *s++;
			else
				s++;
		}
	}
}
const char* CmdLineFile()
{
	if (InitialLoadFileBuffer[0]) {
		char* s = InitialLoadFileBuffer;
		while (*s != 0) {
			s++;
			if (toupper(*(s + 0)) != '.') continue;
			if (toupper(*(s + 1)) != 'D') continue;
			if (toupper(*(s + 2)) != '2') continue;
			if (toupper(*(s + 3)) != 'S') continue;
			return InitialLoadFileBuffer;
		}
		return 0;
	}
	else
		return 0;
}
const char* ProgramFilePath()
{
	return ProgramFilePathBuffer;
}