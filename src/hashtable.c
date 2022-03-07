#include "hashtable.h"

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  if (NULL == newTable) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  newTable->size = size;
  newTable->buckets = malloc(sizeof(struct HashBucketEntry *) * size);
  if (NULL == newTable->buckets) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  for (i = 0; i < size; i++) {
    newTable->buckets[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/* Task 1.2 */
void insertData(HashTable *table, void *key, void *data) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  // 2. Allocate a new hash bucket entry struct.
  // 3. Append to the linked list or create it if it does not yet exist. 
  struct HashBucketEntry *bucketentry, *bucketcur;
  unsigned int index;

  if (table == NULL)
    return;
  if (table->size == 0)
    return;

  bucketentry = malloc(sizeof(struct HashBucketEntry));
  bucketentry->key = key;
  bucketentry->data = data;
  bucketentry->next = NULL;
  index = table->hashFunction(key) % table->size;
  bucketcur = table->buckets[index];

  if (bucketcur == NULL) {
    table->buckets[index] = bucketentry;
    return;
  }
  while(bucketcur->next != NULL) {
      bucketcur = bucketcur->next;
    }
  bucketcur->next = bucketentry;
}

/* Task 1.3 */
void *findData(HashTable *table, void *key) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  // 2. Walk the linked list and check for equality with table->equalFunction.
  struct HashBucketEntry *bucketentry;
  unsigned int index;

  if (table == NULL)
    return NULL;
  if (table->size == 0)
    return NULL;
  index = table->hashFunction(key) % table->size;
  bucketentry = table->buckets[index];

  while (bucketentry != NULL)
  {
    if (table->equalFunction(bucketentry->key, key) == 1) {
      return bucketentry->data;
    }
    bucketentry = bucketentry->next;
  }
  
  return NULL;
}

/* Task 2.1 */
unsigned int stringHash(void *s) {
  // -- TODO --
  //fprintf(stderr, "need to implement stringHash\n");
  /* To suppress compiler warning until you implement this function, */
  unsigned int hash = 0;
  unsigned int seed = 31;
  if (s == NULL)
    return 0;
  char *ch = (char *)s;
  while (*ch != '\0')
  {
    hash = hash * seed + (unsigned int)(*ch);
    ch++;
  }
  
  return hash;
}

/* Task 2.2 */
int stringEquals(void *s1, void *s2) {
  // -- TODO --
  //fprintf(stderr, "You need to implement stringEquals");
  /* To suppress compiler warning until you implement this function */
  char *c1 = (char *)s1;
  char *c2 = (char *)s2;
  int ret = 0;
  if (c1 == NULL || c2 == NULL) 
    return 0;
  while ( !(ret = *c1 - *c2) && *c1) {
      c1++;
      c2++;
  }
  if (ret == 0)
    return 1;
  else
    return 0;
}