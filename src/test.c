#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    FILE *stream;
    char line[100];
    size_t len = 0;
    int i;
    memset(line, 1, 100);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    stream = fopen(argv[1], "r");
    if (stream == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fgets(line, 3, stream) ;

    for(i = 0; i <= 15; i++) {
        printf("%x ", line[i]);
    }
    printf("\n next read\n");

    len = ftell(stream);
    
    printf("cur position: %zd", len);

    fclose(stream);
    exit(EXIT_SUCCESS);
}