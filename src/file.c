#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "file.h"

int
create_db_file (char *filename)
{
    int fd = open (filename, O_RDWR);
    if (fd != -1)
        {
            close (fd);
            puts ("File already exists!");
            return STATUS_ERROR;
        }

    fd = open (filename, O_RDWR | O_CREAT, 0644);
    if (fd == -1)
        {
            perror ("open");
            return STATUS_ERROR;
        }

    return fd;
}

int
open_db_file (char *filename)
{
    int fd = open (filename, O_RDWR, 0644);
    if (fd == -1)
        {
            perror ("open");
            return STATUS_ERROR;
        }

    return fd;
}
