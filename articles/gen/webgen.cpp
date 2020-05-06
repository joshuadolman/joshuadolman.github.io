#include <windows.h>
#include <stdio.h>
#include "types.h"

struct ascii_string {
    s32 length;
    char* data;
};

internal b32
MemFree(void* memory, s64 size)
{
    return VirtualFree(memory, size, MEM_DECOMMIT | MEM_FREE);
}

internal inline b32
MemFree(void* memory)
{
    return MemFree(memory, 0);
}

internal void*
MemAlloc(s64 size)
{
    return VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

// s32 CStrLength(const char* string)
// {
//     return CStrLength((char*)string);
// }

internal s32
NextPowerOf2(s32 n)
{ 
    s32 count = 0; 
    
    // First n in the below condition 
    // is for the case where n is 0 
    if (n && !(n & (n - 1)))
    {
        return n; 
    }
        
    
    while( n != 0) 
    { 
        n >>= 1; 
        count += 1; 
    } 
    
    return 1 << count; 
} 

internal s32
CStrLength(char* string)
{
    s32 length = 0;
    char* At = string;
    while (*At != '\0')
    {
        length++;
        At++;
    }
    return length;
}

class string_list {
public:
    s32 count;
    s32 max_count;
    ascii_string* list;

    void Delete()
    {
        // printf("string_list::delete\n");
        while (this->count > 0)
        {
            this->Remove(0);
        }
        // this->count = 0; // Should be set to 0 by the above while loop
        MemFree(this->list);
        this->max_count = 0; 
    }

    b32 Reduce()
    {
        printf("string_list::Reduce\n");
        if (count==max_count)
        {
            return true;
        }
        s32 desiredCount = NextPowerOf2(count);
        if (desiredCount > max_count)
        {
            return true;
        } else {
            if ( MemFree(this->list + desiredCount, max_count-desiredCount) )
            {
                this->max_count = desiredCount;
                return true;
            } else {
                return false;
            }
        }
    }

    b32 Init(void)
    {
        // printf("string_list::init\n");
        return this->Init(16);
    }

    b32 Init(s32 desiredCount)
    {
        // printf("string_list::init (s32)\n");
        if (this->list)
        {
            this->Delete();
        }

        this->list = (ascii_string*)MemAlloc(sizeof(ascii_string)*desiredCount);
        if (this->list)
        {
            this->max_count = desiredCount;
            return true;
        } else {
            return false;
        }
    }

    b32 Add(char* c_string)
    {
        // printf("string_list::add (c_string)\n");
        ascii_string newString = {};
        newString.length = CStrLength(c_string) + 1;
        if (newString.length < 0)
        {
            this->Error("Length of c_string passed to string_list::Add was invalid (less than 0)");
            printf("        recieved length of %ld\n", newString.length);
            return false;
        }
        newString.data = (char*)MemAlloc(newString.length);
        if (!newString.data)
        {
            return false;
        }
        memcpy(newString.data, c_string, newString.length);

        return this->Add(newString);
    }

    b32 Add(ascii_string newString)
    {
        // printf("string_list::add (ascii_string)\n");
        if (!this->list)
        {
            this->Init();
        }

        if (this->count==this->max_count)
        {
            string_list oldList = *this;
            this->list = 0;
            this->max_count *= 2;
            this->list = (ascii_string*)MemAlloc(sizeof(ascii_string)*this->max_count);
            if (this->list)
            {
                memcpy(this->list, oldList.list, oldList.count*sizeof(ascii_string));
                this->count = oldList.count;
                MemFree(oldList.list);
            } else {
                *this = oldList;
                return false;
            }
        }
        else if (this->count > this->max_count)
        {
            // ERROR - attempted to add ascii_string to string_list but the count is somehow above the max_count which should never happen
            return false;
        }

        this->list[this->count] = newString;
        this->count++;
        return true;
    }

    b32 Remove(s32 index)
    {
        // printf("string_list::remove (s32)\n");
        if ( (index < 0) || (index > this->count-1) || !this->list )
        {
            return false;
        }

        MemFree(this->list[index].data);
        if (index < this->count-1)
        {
            this->list[index] = this->list[this->count-1];
            this->count--;
        }
        return true;
    }

    void Print()
    {
        // printf("string_list::print\n");
        printf("\nstring_list - %ld/%ld :\n", this->count, this->max_count);
        for (int i=0;i < this->count; i++)
        {
            printf("    [%ld]: %s\n", i, this->list[i].data);
        }
        printf("\n");
    }

    void Error(char* error)
    {
        // printf("string_list::error\n");
        printf("    [ERROR]: %s\n", error);
    }
};

int main(int argc, char* argv)
{
    char Buffer[MAX_PATH];
    memset(Buffer, 0, MAX_PATH);
    GetCurrentDirectoryA(MAX_PATH-3, Buffer);
    printf("\nCurrent Dir is:\n");
    printf("    %s\n", Buffer);

    char* At = Buffer;
    while (*At != 0)
    {
        At++;
    }
    *At = '\\';
    *(At+1) = '*';
    *(At+2) = '\0';
    printf("    %s\n\n", Buffer);

    LARGE_INTEGER filesize;
    WIN32_FIND_DATAA FindData = {};
    HANDLE File = FindFirstFileA(Buffer, &FindData);
    if (INVALID_HANDLE_VALUE == File)
    {
        printf("Invalid file handle value\n");
        char ErrorBuffer[256];
        int ErrorID = GetLastError();
        int ErrorWrittenCount = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, ErrorID, MAKELANGID(LANG_ENGLISH, LANG_ENGLISH), ErrorBuffer, 256, NULL);
        printf("ERROR[%ld]: (%ld) %s", ErrorID, ErrorWrittenCount, ErrorBuffer);
        return ErrorID;
    }
    
    int FileCount = 1;
    printf("    [%ld]:", FileCount);
    printf("  %s\n", FindData.cFileName);
    
    while (FindNextFileA(File, &FindData) != 0)
    {
        FileCount++;
        printf("    [%ld]:", FileCount);
        if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            printf("  %s   <DIR>\n", FindData.cFileName);
        } else {
            filesize.LowPart = FindData.nFileSizeLow;
            filesize.HighPart = FindData.nFileSizeHigh;
            printf("  %s   %lld bytes\n", FindData.cFileName, filesize.QuadPart);
        }
    }

    printf("%zd\n", sizeof(string_list));
    string_list List = {};
    List.Init(4);
    List.Add("yo");
    List.Add("hey im a string");
    List.Add("hello binches");
    List.Add("oh no");
    List.Print();
    List.Add("did it work?");
    List.Print();
    List.Remove(2);
    List.Remove(0);
    List.Print();
    List.Reduce();
    List.Print();

    return 0;
}