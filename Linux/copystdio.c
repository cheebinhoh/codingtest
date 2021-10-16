/* Copyright © 2021 Chee Bin HOH. All rights reserved.
 *
 * Copy standard input to output or files in argument.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char *argv[])
{
    char buf[BUFSIZ];
    int  n;


    while ( ( n = read(STDIN_FILENO, buf, sizeof(buf) ) ) > 0 )
    {
        if ( write(STDOUT_FILENO, buf, n) != n )
        {
            fprintf(stderr, "write error: %s\n", strerror(errno));
            exit(1);
        }    
    }

    if ( n < 0 )
    {
        fprintf(stderr, "read error: %s\n", strerror(errno));
        exit(1);
    }
    
    return 0;
}
