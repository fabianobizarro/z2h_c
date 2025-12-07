#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

int
create_db_header (struct dbheader_t **headerOut)
{
    struct dbheader_t *header = calloc (1, sizeof (struct dbheader_t));

    if (header == NULL)
        {
            puts ("Malloc failed to create db header");
            return STATUS_ERROR;
        }

    header->version = 0x1;
    header->count = 0;
    header->magic = HEADER_MAGIC;
    header->filesize = sizeof (struct dbheader_t);

    *headerOut = header;

    return STATUS_SUCCESS;
}

int
validate_db_header (int fd, struct dbheader_t **headerOut)
{
    if (fd < 0)
        {
            puts ("Invalid File Descriptor from the user.");
            return STATUS_ERROR;
        }

    struct dbheader_t *header = calloc (1, sizeof (struct dbheader_t));
    if (header == NULL)
        {
            puts ("Malloc failed to create a db header");
            return STATUS_ERROR;
        }

    if (read (fd, header, sizeof (struct dbheader_t))
        != sizeof (struct dbheader_t))
        {
            perror ("read");
            free (header);
            return STATUS_ERROR;
        }

    header->version = ntohs (header->version);
    header->count = ntohs (header->count);
    header->magic = ntohl (header->magic);
    header->filesize = ntohl (header->filesize);

    if (header->magic != HEADER_MAGIC)
        {
            puts ("Improper header magic");
            free (header);

            return STATUS_ERROR;
        }

    if (header->version != 1)
        {
            puts ("Improper header version");
            free (header);

            return STATUS_ERROR;
        }

    struct stat dbstat = { 0 };
    fstat (fd, &dbstat);
    if (header->filesize != dbstat.st_size)
        {
            puts ("Corrupted database");
            free (header);

            return STATUS_ERROR;
        }

    *headerOut = header;

    return STATUS_SUCCESS;
}

int
output_file (int fd, struct dbheader_t *header, struct employee_t *employees)
{
    if (fd < 0)
        {
            puts ("Got a bad FD from the user");
            return STATUS_ERROR;
        }

    int realcount = header->count;

    header->magic = htonl (header->magic);
    header->filesize = htonl (sizeof (struct dbheader_t)
                              + (sizeof (struct employee_t) * realcount));
    header->count = htons (header->count);
    header->version = htons (header->version);

    lseek (fd, 0, SEEK_SET);

    write (fd, header, sizeof (struct dbheader_t));

    int i = 0;
    for (; i < realcount; i++)
        {
            employees[i].hours = htonl (employees[i].hours);
            write (fd, &employees[i], sizeof (struct employee_t));
        }

    return STATUS_SUCCESS;
}

int
read_employees (int fd, struct dbheader_t *header,
                struct employee_t **employeesOut)
{
    if (fd < 0)
        {
            puts ("Got a bad FD from the user");
            return STATUS_ERROR;
        }

    int count = header->count;

    struct employee_t *employees = calloc (count, sizeof (struct employee_t));

    if (employees == NULL)
        {
            puts ("Malloc failed");

            return STATUS_ERROR;
        }

    read (fd, employees, count * sizeof (struct employee_t));

    for (int i = 0; i < count; i++)
        {
            employees[i].hours = ntohl (employees[i].hours);
        }

    *employeesOut = employees;

    return STATUS_SUCCESS;
}

int
add_employee (struct dbheader_t *header, struct employee_t **employees,
              char *addstring)
{
    if (NULL == header)
        return STATUS_ERROR;
    if (header->count < 0)
        return STATUS_ERROR;
    if (NULL == employees)
        return STATUS_ERROR;
    if (NULL == *employees)
        return STATUS_ERROR;
    if (NULL == addstring)
        return STATUS_ERROR;

    char *name = strtok (addstring, ",");
    if (name == NULL)
        return STATUS_ERROR;

    char *addr = strtok (NULL, ",");
    if (addr == NULL)
        return STATUS_ERROR;

    char *hours = strtok (NULL, ",");
    if (hours == NULL)
        return STATUS_ERROR;

    struct employee_t *e = *employees;
    e = realloc (e, sizeof (struct employee_t) * header->count + 1);
    if (e == NULL)
        {
            return STATUS_ERROR;
        }

    header->count++;

    strncpy (e[header->count - 1].name, name,
             sizeof (e[header->count - 1].name) - 1);
    strncpy (e[header->count - 1].address, addr,
             sizeof (e[header->count - 1].address) - 1);

    e[header->count - 1].hours = atoi (hours);

    *employees = e;

    return STATUS_SUCCESS;
}

void
list_employees (struct dbheader_t *dbheader, struct employee_t *employees)
{
    int i = 0;
    puts ("=== List of employees ===");
    for (; i < dbheader->count; i++)
        {
            printf ("Employee %i\n", i);
            printf ("\tName: %s\n", employees[i].name);
            printf ("\tAddress: %s\n", employees[i].address);
            printf ("\tHours: %i\n", employees[i].hours);
        }
}
