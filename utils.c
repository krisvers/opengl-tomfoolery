#include <utils.h>
#include <types.h>
#include <stdio.h>
#include <stdlib.h>

void * new(u64 size) {
	void * n = malloc(size);
	if (n == NULL) {
		fprintf(stderr, "new(): object of size %llu failed to allocate\n", (long long unsigned int) size);
		abort();
	}

	return n;
}
