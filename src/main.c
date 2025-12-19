#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void
print_usage (char **argv)
{
    printf ("Usage: %s -n -f <database file>\n", argv[0]);
    puts ("\t -n - create new database file");
    puts ("\t -f - (required) path to database file");
    puts ("\t -l - list the empployees");
    puts ("\t -a - add cia CSV of (name,address,hours)");
    puts ("\t -d - delete an employee by name");
}

int
main (int argc, char *argv[])
{
    int c;
    int dbfd = -1;
    bool newfile = false;
    bool list = false;
    char *filepath = NULL;
    char *addstring = NULL;
    char *deletestring = NULL;
    struct dbheader_t *dbheader = NULL;
    struct employee_t *employees = NULL;

    while ((c = getopt (argc, argv, "nf:a:ld:")) != -1)
        {
            switch (c)
                {
                case 'n':
                    newfile = true;
                    break;
                case 'f':
                    filepath = optarg;
                    break;
                case 'a':
                    addstring = optarg;
                    break;
                case 'l':
                    list = true;
                    break;
                case 'd':
                    deletestring = optarg;
                    break;
                case '?':
                    printf ("Unknown options -%c\n", c);
                    break;
                default:
                    return -1;
                }
        }

    if (filepath == NULL)
        {
            puts ("Filepath is a required argument");
            print_usage (argv);

            return 0;
        }

    if (newfile)
        {
            dbfd = create_db_file (filepath);
            if (dbfd == STATUS_ERROR)
                {
                    puts ("Unable to create database file.");
                    return -1;
                }

            if (create_db_header (&dbheader) == STATUS_ERROR)
                {
                    puts ("Failed to create database header!");
                    return -1;
                }
        }
    else
        {
            dbfd = open_db_file (filepath);
            if (dbfd == STATUS_ERROR)
                {
                    puts ("Unable to create database file.");
                    return -1;
                }

            if (validate_db_header (dbfd, &dbheader) == STATUS_ERROR)
                {
                    puts ("Failed to validate database header");
                    return -1;
                }
        }

    if (read_employees (dbfd, dbheader, &employees) != STATUS_SUCCESS)
        {
            puts ("Failed to read employees");
            return -1;
        }

    if (addstring)
        {
            printf ("before addind emp - magic: %i\n", dbheader->magic);
            if (add_employee (dbheader, &employees, addstring)
                != STATUS_SUCCESS)
                {
                    puts ("Failed to add the employee");

                    return -1;
                }
        }

    if (deletestring)
        {
            if (remove_employee (dbheader, &employees, deletestring)
                != STATUS_SUCCESS)
                {
                    puts ("Failed to remove the employee");
                }
            else
                {
                    printf ("Employee %s removed successfully.\n",
                            deletestring);
                }
        }

    if (list)
        {
            list_employees (dbheader, employees);
        }

    output_file (dbfd, dbheader, employees);

    return 0;
}
