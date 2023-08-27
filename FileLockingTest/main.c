#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"

#define FAIL 1
#define SUCCESS 0

static char const buffer[17] = "A line of text.\n";
static size_t const buffer_length = sizeof(buffer);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: FileWriter.exe <filename>\n");
        return FAIL;
    }

    char* filename = argv[1];
    FILE* f = fopen(filename, "wb+");
    if (f == NULL)
    {
        printf("Failed to create file\n");
        return FAIL;
    }

    printf("File \"%s\" created successfully\n", filename);

    char command_string[1024];
    for (;;)
    {
        printf("Press w to write or q to close...\n");
        scanf("%s", command_string);
        char const command = command_string[0];

        if (command == 'w' || command == 'W')
        {
            size_t const bytes_written = fwrite(buffer, sizeof(char), buffer_length, f);
            if (fflush(f) == SUCCESS)
            {
                printf("Wrote %llu bytes to file.\n", bytes_written);
            }
            else
            {
                printf("Failed to flush to file.\n");
            }
        }
        else if (command == 'q' || command == 'Q')
        {
            break;
        }
    }

    if (fclose(f) != SUCCESS)
    {
        printf("Failed to close file\n");
        return FAIL;
    }

    return SUCCESS;
}