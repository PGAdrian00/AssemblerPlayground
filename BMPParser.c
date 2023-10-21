#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push)
#pragma pack(1)
typedef struct pixel pixel;
struct pixel
{
    BYTE R;
    BYTE G;
    BYTE B;
};



#pragma pack(pop)

int main(int argc, char** argv)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    DWORD fileSizeHigh = 0;
    DWORD fileSizeLow = 0;
    DWORD status = 0;
    HANDLE hMapping = INVALID_HANDLE_VALUE;

    //usage bmpparser.exe <path>
    //argv[0] - bmpparser.exe
    //argv[1] - argument
    if (2 != argc)
    {
        printf("Invalid nr of arguments\n");
        return 1;
    }

    int a = sizeof(struct pixel);

    hFile = CreateFileA(argv[1],
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (INVALID_HANDLE_VALUE == hFile)
    {
        printf("Could not open file. Error code: %d\n", GetLastError());
        return 2;
    }

    fileSizeLow = GetFileSize(hFile, &fileSizeHigh);
    if (INVALID_FILE_SIZE == fileSizeLow)
    {
        printf("Could not obtain size of file. Error: %d\n", GetLastError());
        status = 3;
        goto cleanup;
    }

    hMapping = CreateFileMappingA(hFile,
        NULL,
        PAGE_READWRITE,
        fileSizeHigh, fileSizeLow,
        NULL);
    if (INVALID_HANDLE_VALUE == hMapping)
    {
        printf("Could not open mapping to file. Error code: %d\n", GetLastError());
        status = 4;
        goto cleanup;
    }

    BYTE* fileBuffer = NULL;

    fileBuffer = MapViewOfFile(hMapping,
        FILE_MAP_ALL_ACCESS,
        0, 0,
        fileSizeLow);
    if (NULL == fileBuffer)
    {
        printf("Could not map buffer in memory. Error code: %d\n", GetLastError());
        status = 5;
        goto cleanup;
    }

    if (fileBuffer[0] != 'B' && fileBuffer[1] != 'M')
    {
        printf("Not a bitmap\n");
        status = 6;
        goto cleanup;
    }
    else {
        printf("Opened file is of bitmap format!\n");
    }

    //pixeli = (DWORD*) &fileBuffer[10] //se gaseste un DWORD cu o valoare de unde incep pixelii
    //fileBuffer[pixeli]
    //sa pun astea direct ca adresa la pixeli
    DWORD pixels_offset = *(DWORD*)(fileBuffer +10);
    DWORD width = *(DWORD*)(fileBuffer + 18);
    DWORD height = *(DWORD*)(fileBuffer + 22);
    if (height < 0) {
        printf("We want to open files with positive height!\n");
        status = 7;
        goto cleanup;
    }

    WORD bitCount = *(WORD*)(fileBuffer + 28);

    

    //memcpy(&pixels_offset, &fileBuffer[10], sizeof(DWORD));
    //memcpy(&width, &fileBuffer[18], sizeof(DWORD));
    //memcpy(&height, &fileBuffer[22], sizeof(DWORD));
    
    BYTE* pixelsData = fileBuffer + *(DWORD*)(fileBuffer + 10);

    printf("File dimensions found at offsets 18 width and 22 height: %d x %d \n", width, height);
    printf("Pixels offset found at offset 10: %d \n", pixels_offset);
    printf("Bitcound found at offset 28: %d\n", bitCount);
    

    BOOL shouldExit = FALSE;

    for (DWORD y = height - 1; y >= 0 && !shouldExit; y--) {
        if (y <= 0) { 
            shouldExit = TRUE;
            break; 
        }
        for (DWORD x = 0; x < width; x++) {
            pixel* currentPixel = (pixel*)(pixelsData + (x + y * width) * sizeof(pixel));

            BYTE temp = currentPixel->R;
            currentPixel->R = currentPixel->B;
            currentPixel->B = temp;


            BYTE red = currentPixel->R;
            BYTE green = currentPixel->G;
            BYTE blue = currentPixel->B;

            printf("Pixel at (%d, %d): R=%d, G=%d, B=%d\n", x, y, red, green, blue);
        }
            
            
    }
    


cleanup:
    CloseHandle(hFile);
    CloseHandle(hMapping);

    return status;
}