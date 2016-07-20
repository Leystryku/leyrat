
extern "C" int stricmp(const char*ch1, const char*ch2);

extern "C" int _stricmp(const char*ch1, const char*ch2)
{
	return stricmp(ch1, ch2);
}

extern "C" int isdigit(char ch)
{
	return ch > 47 && ch < 58;
}

extern "C" int _strnicmp(const char*str1, const char*str2, unsigned int cnt)
{
	int ret = stricmp(str1, str2);

	return ret;
}

extern "C" unsigned int strlen(const char*ch);
extern "C" char* strcpy(char*dest, const char*src);

extern "C" char* strdup(const char *s)
{
	char *d = new char[strlen(s) + 1];   // Space for length plus nul
	if (d == 0) return 0;          // No memory
	strcpy(d, s);                        // Copy the characters
	return d;                            // Return the new string
}

extern "C" char* strtok(char *string, const char *delimiter);

extern "C" char* strtok_s(char *string, const char *delimiter, char **context)
{
	return strtok(string, delimiter);
}

extern "C" void* __stdcall CreateThread( void* lpThreadAttributes, unsigned long dwStackSize, void* lpStartAddress, void* lpParameter, unsigned long dwCreationFlags, unsigned long* lpThreadId );

extern "C" unsigned long _beginthreadex(
	void* security, // security attribute
	unsigned stackSize, // size of the new thread’s stack
	unsigned(__stdcall *funcstart) (void *), // starting address of function to run
	void* argList, // arguments to be passed to the thread
	unsigned initFlags, // initial state of the thread: running or suspended
	unsigned long* threadAddr // thread ID
	)
{
	return (unsigned long)CreateThread(security, stackSize, funcstart, argList, initFlags, threadAddr);
}


typedef struct _iobuf
{
	void* _Placeholder;
} FILE;

extern "C" FILE* __cdecl __acrt_iob_func(unsigned int)
{
	return 0;//???, they return stdin/
}


extern "C" int _wrename(const wchar_t*oldname, const wchar_t* newname)
{
	return 0;
}

extern "C" FILE* _wfopen(const wchar_t *filename, const wchar_t *mode)
{
	return 0;
}

extern "C" int _wunlink(const wchar_t *filename)
{
	return 0;
}

extern "C" int _fseeki64(FILE*handle, long long offset, int origin)
{
	return 0;
}

extern "C" long long _ftelli64(FILE*handle)
{
	return 0;
}

extern "C" void _ftime64(void*timeptr)
{

}

extern "C" int _wstat64(const wchar_t *path, void*buffer)
{
	return 0;
}

extern "C" void _aulldiv()
{

}

extern "C" void _aullrem()
{

}

extern "C" void _allmul()
{
}