#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
  if (map == NULL)
    return;
  long pos = hash(key, map->capacity);
  while (map->buckets[pos] != NULL && map->buckets[pos]->key != NULL) {
    if (is_equal(map->buckets[pos]->key, key)) {
      map->buckets[pos]->value = value;
      return;
    }
    pos = (pos + 1) % map->capacity;
  }
  if (map->size == map->capacity) {
    enlarge(map);
    pos = hash(key, map->capacity);
  }
  Pair *n = createPair(key, value);
  map->buckets[pos] = n;
  map->size++;
  map->current = pos;
}

void enlarge(HashMap *map) {
  enlarge_called = 1; // no borrar (testing purposes)
}

HashMap *createMap(long capacity) {
  HashMap *map = (HashMap *)malloc(sizeof(HashMap));
  if (map == NULL)
    return NULL;
  map->buckets = (Pair **)malloc(sizeof(Pair *) * capacity);
  if (map->buckets == NULL)
    return NULL;
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
}

void eraseMap(HashMap *map, char *key) {
  if (map == NULL || map->buckets == NULL || key == NULL)
    return;
  long pos = hash(key, map->capacity);
  while (map->buckets[pos] != NULL &&
         (map->buckets[pos]->key == NULL ||
          strcmp(map->buckets[pos]->key, key) != 0)) {
    pos = (pos + 1) % map->capacity;
  }
  if (map->buckets[pos] != NULL && strcmp(map->buckets[pos]->key, key) == 0) {
    map->buckets[pos]->key = NULL;
    map->buckets[pos]->value = NULL;
    map->size--;
  }
}

Pair *searchMap(HashMap *map, char *key) {

  long pos = hash(key, map->capacity);
  // long aux;
  if (map->buckets[pos]->key == key) {
    return map->buckets[pos];
  }
  while (map->buckets[pos] != NULL) {
    if (strcmp(map->buckets[pos]->key, key) == 0) {
      map->current = pos;
      return map->buckets[pos];
    }
    pos = (pos + 1) % map->capacity;
  }
  return map->buckets[pos];
}

Pair *firstMap(HashMap *map) {
  for (long i = 0; i < map->capacity; i++) {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
      return map->buckets[i];
    }
  }
  return NULL;
}

Pair *nextMap(HashMap *map) { return NULL; }
