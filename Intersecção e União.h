#ifndef TR67_SUAMATRICULA_H
#define TR67_SUAMATRICULA_H

#include <stdlib.h>

typedef struct {
  int *elements;
  int size, maximum;
  float loadFactor;
} Set;

Set *emptySet();
Set *readSet(FILE *file);
void insert(Set *set, int element);
void print(Set *set);
void writeSet(FILE *file, Set *set);
void unionSet(Set *setC, Set *setA, Set *setB);
void intersecSet(Set *setC, Set *setA, Set *setB);

#endif