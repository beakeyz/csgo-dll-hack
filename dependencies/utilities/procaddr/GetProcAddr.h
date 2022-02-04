#pragma once

//NOTE: nice try idiot, ofc this didn't work
// better luck next time

#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include "intrin.h"
#include <malloc.h>

bool LibraryLoaded(wchar_t* dllName);
void* GetProcAddressCustom(wchar_t* dllName, wchar_t* procName);
void* _LoadLibraryW(wchar_t* dllName);
bool StringContains(wchar_t* haystack, wchar_t* needle);
bool StringMatches(wchar_t* str1, wchar_t* str2);
int StringLengthW(wchar_t* str);
int StringLengthA(char* str);
wchar_t* CharToWChar_T(char* str);
wchar_t** ParseForwardString(char* str);
wchar_t ToLowerW(wchar_t ch);
char ToLowerA(char ch);