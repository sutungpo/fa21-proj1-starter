/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
#ifndef _PHILPHIX_UNITTEST
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}
#endif /* _PHILPHIX_UNITTEST */

#define BUF 1024
char *getnewline(FILE *stream) {
  char *buffer = NULL;
  size_t size = 0;
  char newline_found = 0, found = 0;
  for (;;)
  {
    char *temp = realloc(buffer, size + BUF);
    if (!temp)
    {
            free(buffer);
            return NULL;
    }
    buffer = temp;

    if (!fgets(buffer + size, BUF, stream))
    {
            free(buffer);
            return NULL;
    }

    if(strchr(buffer + size, '\n') || feof(stream))
      newline_found = 1;
    if (!newline_found)
            size += BUF - 1;
    else
            break;

  }

  return buffer;
}

/* Task 3 */
void readDictionary(char *dictName) {
  // -- TODO --
  //fprintf(stderr, "You need to implement readDictionary\n");
  FILE *fp;
  char *m, *n;
  char *buf, string;
  int len = 0, found = 0;
  int startstr = -1, ix = 0, preblank = 0, endstr = 0;

  if ( (fp = fopen(dictName, "r")) == NULL) {
    puts("failed to open file!");
    exit(61);
  }

  while ((buf = getnewline(fp)) != NULL) {
    len = strlen(buf);
    while(1) {
      string = buf[ix];
      if (string != ' ' && string != '\t' && string != '\n' && string != '\0') {
        if (startstr == -1)
          startstr = ix;
        if (preblank) {
          startstr = ix;
          preblank = 0;
        }
      }
      else if (ix <= len) {
          if (!preblank) {
            endstr = ix;
            preblank = 1;
            if (found == 0) {
              m = calloc(endstr - startstr + 1, sizeof(char));
              memmove(m, &buf[startstr], endstr - startstr);
              found = 1;
            }
            else if (found == 1) {
              n = calloc(endstr - startstr + 1, sizeof(char));
              memmove(n, &buf[startstr], endstr - startstr);
              insertData(dictionary, m ,n);
              found = 0;
              break;
            }
          }
          if (ix == len) {
            break;
          }
      }
      ix++;
    }
    free(buf);
    len = 0, startstr = -1, ix = 0, preblank = 0, endstr = 0;
  }

  fclose(fp);
}

char *varcheck(char *word1, int len) {
  char *w1, *w2, *w3, *data;
  int delta, ix = 0;

  w1 = calloc(len + 1, sizeof(char));
  w2 = calloc(len + 1, sizeof(char));
  w3 = calloc(len + 1, sizeof(char));
  memmove(w1, word1, len);

  while(ix < len) {
    delta = 0;
    if (w1[ix] >= 'A' && w1[ix] <= 'Z') {
      delta = 0x20;
    }
    w3[ix] = w1[ix] + delta;
    ix++;
  }
  memmove(w2, w3, len);
  if (w1[0] >= 'A' && w1[0] <= 'Z') {
    w2[0] = w1[0];
  }

  data = findData(dictionary, w1);
  if (data == NULL) {
    data = findData(dictionary, w2);
  } 
  if (data == NULL) {
    data = findData(dictionary, w3);
  } 
  if (data == NULL) {
    data = word1;
  } 
  
  free(w1);
  free(w2);
  free(w3);

  return data;
}

/* Task 4 */
void processInput() {
  // -- TODO --
  // fprintf(stderr, "You need to implement processInput\n");
  char *buf, *bufback, *w, string;
  int len = 0,  startstr = -1, endstr = 0, preblank = 0, ix = 0, ixb = 0, lenback;

  while ((buf = getnewline(stdin)) != NULL) {
    len = strlen(buf);
    lenback = len + 1;
    bufback = malloc(lenback);
    while(1) {
      string = buf[ix];
      if ((string >= '0' && string <= '9') || (string >= 'A' && string <= 'Z') || (string >= 'a' && string <= 'z')) {
        if (startstr == -1)
          startstr = ix;
        if (preblank) {
          startstr = ix;
          preblank = 0;
          if (ixb + startstr - endstr > 3 * lenback / 4) {
            lenback = 2 * lenback;
            bufback = realloc(bufback, lenback);
          }
          memmove(&bufback[ixb], &buf[endstr], startstr - endstr);
          ixb += startstr - endstr;
        }
      }
      else if (ix <= len) {
        if (!preblank) {
          endstr = ix;
          preblank = 1;
          w = varcheck(&buf[startstr], endstr - startstr);
          if (w != &buf[startstr]) {
            if (ixb + strlen(w) > 3 * lenback / 4) {
              lenback = 2 * lenback;
              bufback = realloc(bufback, lenback);
            }
            memmove(&bufback[ixb], w, strlen(w));
            ixb += strlen(w);
          }
          else {
            if (ixb + endstr - startstr > 3 * lenback / 4) {
              lenback = 2 * lenback;
              bufback = realloc(bufback, lenback);
            }
            memmove(&bufback[ixb], w, endstr - startstr);
            ixb += endstr - startstr;
          }

          //if (ix == len)
          //  bufback[ixb] = '\0';
        }
        if (ix == len) {
          if (ixb + ix - endstr + 1 > 3 * lenback / 4) {
            lenback = 2 * lenback;
            bufback = realloc(bufback, lenback);
          }
          memmove(&bufback[ixb], &buf[endstr], ix - endstr + 1);
          break;
        }
      }
      ix++;
    }
    fputs(bufback, stdout);
    startstr = -1, endstr = 0, preblank = 0, ix = 0, ixb = 0;
    free(buf);
    free(bufback);
  }
}
