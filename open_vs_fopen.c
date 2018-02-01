//gcc -o open_vs_fopen open_vs_fopen.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>
#include "slo_timers.h"

void main (int argc, char **argv)
{
    char *filename;
    if (argc < 2) {
        printf ("Usage: open_vs_fopen <filename> [1|2]\n");
    } else {
        filename = argv[1];
    }

    struct stat file_info;
    char *buff;

    stat (filename, &file_info);
    buff = malloc (file_info.st_size);
    printf ("loading %ld bytes\n", file_info.st_size);

    setup_clocks ();
    if (argc == 3 && strcmp(argv[2], "2")) {
        printf ("open():\n");
        {
            BEGIN_TIME_BLOCK;
            int f = open (filename, O_RDWR|O_CREAT, 0666);
            END_TIME_BLOCK("File open.");

            uint32_t bytes_read = 0;
            BEGIN_TIME_BLOCK;
            while (bytes_read < file_info.st_size) {
                bytes_read += read (f, buff, file_info.st_size);
            }
            END_TIME_BLOCK("File read.");
            assert (bytes_read == file_info.st_size && "Did not read complete file.");

            BEGIN_TIME_BLOCK;
            close (f);
            END_TIME_BLOCK("File close.");
        }
    } else {
        printf ("fopen():\n");
        {
            BEGIN_TIME_BLOCK;
            FILE *f = fopen (filename, "r");
            END_TIME_BLOCK("File open.");

            uint32_t bytes_read = 0;
            BEGIN_TIME_BLOCK;
            bytes_read = fread (buff, 1, file_info.st_size, f);
            END_TIME_BLOCK("File read.");
            assert (bytes_read == file_info.st_size && "Did not read complete file.");

            BEGIN_TIME_BLOCK;
            fclose (f);
            END_TIME_BLOCK("File close.");
        }
    }
}
