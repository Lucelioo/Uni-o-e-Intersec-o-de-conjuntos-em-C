#include <stdio.h>
#include <stdlib.h>
#include "TR67_538531.h"

Set *emptySet() {
  Set *set = (Set*) malloc(sizeof(Set));
  set->size = 0;
  set->maximum = 0;
  set->loadFactor = 0.0;
  set->elements = NULL;

  return set;
}

void print(Set *set) {
  printf("Size: %d\n", set->size);
  printf("Maximum: %d\n", set->maximum);
  printf("Factor: %f\n", set->loadFactor);

  for (int index = 0; index < set->maximum / 32 + 1; index++) {
    for (int bit = 0; bit < 32; bit++) {
      if (set->elements[index] & (1 << bit)) {
        printf("%d - ", (index << 5) + bit);
      }
    }
  }

  printf("\n");
}

int highestBit(unsigned int number) {
  int bit = 0;
  while (number >>= 1) bit++;

  return bit;
}

int countOnes(unsigned int size) {
  unsigned int counter;

  counter = size - ((size >> 1) & 033333333333) - ((size >> 2) & 011111111111);
  return ((counter + (counter >> 3)) & 030707070707) % 63;
}

void insert(Set *set, int element) {
  set->elements[element >> 5] |= (1 << (element & 31));
}

Set *readSet(FILE *file) {
  Set *set = emptySet();

  if (file != NULL) {
    char buffer[256];

    fscanf(file, "%d %d %f", &set->size, &set->maximum, &set->loadFactor);

    set->elements = (int*) calloc(set->maximum / 32 + 1, sizeof(int));

    while (fscanf(file, "%s", buffer) != EOF) {
      insert(set, atoi(buffer));
    }
  }

  return set;
}

void writeSet(FILE *file, Set *set) {
  if (set->size == 0) {
    fprintf(file, "%d\n%d\n%f\n", 0, 0, 0.0);
  } else {
    fprintf(file, "%d\n%d\n%f\n", set->size, set->maximum, set->loadFactor);

    for (int index = 0; index < set->maximum / 32 + 1; index++) {
      for (int bit = 0; bit < 32; bit++) {
        if (set->elements[index] & (1 << bit)) {
          fprintf(file, "%d\n", (index << 5) + bit);
        }
      }
    }
  }
}

void unionSet(Set *setC, Set *setA, Set *setB) {
  Set *major = setA->maximum > setB->maximum ? setA : setB;
	Set *minor = setA->maximum > setB->maximum ? setB : setA;

	setC->maximum = major->maximum;
	setC->elements = (int*) calloc((setC->maximum >> 5) + 1, sizeof(int));

  for (int index = 0; index < (major->maximum >> 5) + 1; index++) {
    setC->elements[index] = major->elements[index];
		if ((minor->maximum >> 5) + 1 > index) {
			setC->elements[index] |= minor->elements[index];
		}

		setC->size += countOnes(setC->elements[index]);
  }

  if (setC->maximum > 0) setC->loadFactor = (float) setC->size / (float) setC->maximum;
}

void intersecSet(Set *setC, Set *setA, Set *setB) {
  Set *major = setA->maximum > setB->maximum ? setA : setB;
	Set *minor = setA->maximum > setB->maximum ? setB : setA;

	setC->elements = (int*) calloc((minor->maximum >> 5) + 1, sizeof(int));

  for (int index = (minor->maximum >> 5); index >= 0; index--) {
    setC->elements[index] = minor->elements[index] & major->elements[index];
		setC->size += countOnes(setC->elements[index]);

		if (setC->maximum == 0 && setC->elements[index] != 0) {
		 	setC->maximum = (index << 5) + highestBit(setC->elements[index]);
		}
  }

	if (setC->maximum > 0) setC->loadFactor = (float) setC->size / (float) setC->maximum;
}