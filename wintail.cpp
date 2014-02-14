
#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
    Wow64DisableWow64FsRedirection(NULL);

    FILE* input;
    if (argc == 2) {
        input = _wfopen(argv[1], L"rb");
        if (input == NULL) {
            perror("error opening file");
            return 1;
        }
    } 
    else {
        printf("Usage: wintail [filename]\n");
        return 1;
    }
    bool fromStart = false;
    FILE* output = stdout;

    const int bfSize = 1024;
    char bf[bfSize];
    long outSize = fromStart ? 0 : -1;
    do {
        if (fseek(input, 0, SEEK_END) != 0) {
            perror("error seeking end of file");
            return 1;
        }
        long inSize = ftell(input);
        if (outSize == -1) { outSize = inSize; }
        if (inSize > outSize) {
            if (fseek(input, outSize, SEEK_SET) != 0) {
                perror("error seeking");
                return 1;
            }

            // write content to output
            for (int idx = outSize; idx < inSize; idx += bfSize) {
                int readSize = idx + bfSize > inSize ? inSize - idx : bfSize;
                int resultSize = fread(bf, 1, readSize, input);
                if (resultSize != readSize) {
                    perror("error reading input");
                    return 1;
                }
                int writeSize = fwrite(bf, 1, readSize, output);
                if (writeSize != readSize) {
                    perror("error writing to output");
                    return 1;
                }
            }
            outSize = inSize;
        }
        else {
            Sleep(50);
        }
    } while (true);

    return 0;
}

