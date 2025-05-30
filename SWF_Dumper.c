// SWF_Dumper.c : Simple program that dumps swf flash file from standalone Flash Player projector executable.
// by zbieracz2000

#include <stdio.h>
#include <Windows.h>

int main(int argc, char* argv[])
{
	int     szFile;
    char    dataEnd[8];
    DWORD   lpNumberOfBytesRW;
    HANDLE  hfInput,
            hfOutput,
            hMemHeap,
            hHeap;
	printf("SWF Dumper by zbieracz2000 v1.0\r\n");
    if (argc==3)
	{
    hfInput = CreateFileA(argv[1], GENERIC_READ, 0, NULL,\
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfInput == INVALID_HANDLE_VALUE) {
        printf("Cannot open input file for reading!\r\n");
    } else {
        szFile = GetFileSize(hfInput, NULL);
        SetFilePointer(hfInput, szFile-8, NULL, FILE_BEGIN);
        ReadFile(hfInput, dataEnd, 8, &lpNumberOfBytesRW, NULL);
        if (*(DWORD*)&dataEnd == 0xFA123456) { // Check the signature
            hHeap = GetProcessHeap();
            if (hHeap) {
                hMemHeap = (PHANDLE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, *(DWORD*)&dataEnd[4]);
                SetFilePointer(hfInput, szFile-8-*(DWORD*)&dataEnd[4], NULL, FILE_BEGIN);
                ReadFile(hfInput, hMemHeap, *(DWORD*)&dataEnd[4], &lpNumberOfBytesRW, NULL);
                hfOutput = CreateFileA(argv[2], GENERIC_WRITE, 0, NULL,\
                                    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hfOutput == INVALID_HANDLE_VALUE) {
                    printf("Cannot create output file for writing!\r\n");
                } else {
                    WriteFile(hfOutput, hMemHeap, *(DWORD*)&dataEnd[4], &lpNumberOfBytesRW, NULL);
                    CloseHandle(hfOutput);
                    printf("Done, check your shiny new ");
					printf(argv[2]);
					printf("! =)\r\n");
                }
                HeapFree(hHeap, 0, hMemHeap);
            } else {
                printf("Can't fetch the process heap!\r\n");
            }
        } else {
            printf("This doesn't seems to be a projector bundle.\r\n");
        }
        CloseHandle(hfInput);
    }
	}
	else
		printf("Usage: swf_dumper.exe input.exe output.swf");
	return 0;
}

