#include <windows.h>
#include <stdio.h>

#include "types.h"
#include "jpd_str.cpp"
#include "html_templates.h"

#define VERSION_NUMBER 32

int main()
{
	// string_builder Builder = {};
	// Builder.Init(256);

	// Builder.AppendCStr()
	char* RelativePathToRoot = "";
	int CSS_StringSize = snprintf(NULL, 0, CSS_STRING, RelativePathToRoot, VERSION_NUMBER);
	char* CSS_String = (char*)malloc(CSS_StringSize + 1);
	snprintf(CSS_String, CSS_StringSize+1, CSS_STRING, RelativePathToRoot, VERSION_NUMBER);
	// printf(sectionPageHeader, "IMCSSBITCH!");
	// printf(CSS_String);

	int SectionPageSize = snprintf(NULL, 0, SECTION_PAGE, CSS_String, "Joshua Dolman");
	char* SectionPageString = (char*)malloc(SectionPageSize + 1);
	snprintf(SectionPageString, SectionPageSize + 1, SECTION_PAGE, CSS_String, "Joshua Dolman");
	printf(SectionPageString);


	// char cCurrentPath[FILENAME_MAX];

	string_builder CurrentPath = {};
	CurrentPath.Init(MAX_PATH+2);

	GetCurrentDirectoryA((DWORD)CurrentPath.String.DataSize, CurrentPath.String.Data);
	CurrentPath.String.Length = CStr::Length(CurrentPath.String.Data);

	CurrentPath.AppendCharacter('\\');
	printf ("The current working directory is %s", CurrentPath.String.Data);

	WIN32_FIND_DATA FindData = {};
	auto FoundFile = FindFirstFile(CurrentPath.String.Data, &FindData);
	if (FoundFile)
	{
		printf("\noh no\n");
	} else {
		printf("\nnot found\n");
	}
	printf(FindData.cFileName);
	printf("%d", FindData.nFileSizeLow);

	char FoundPath[MAX_PATH];
	GetFinalPathNameByHandle(FoundFile, FoundPath, MAX_PATH, FILE_NAME_NORMALIZED);
	printf("FoundPath: %s\n", FoundPath);

	do {
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			printf(FindData.cFileName);
			printf("\n");
		}
	}
	while(FindNextFile(FoundFile, &FindData) != 0);

	printf("\n");
}