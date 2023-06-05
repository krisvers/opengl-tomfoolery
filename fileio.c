#include <fileio.h>
#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

u64 file_get_len(FILE * fp) {
	if (fp == NULL) {
		fprintf(stderr, "file_get_len(): given file descriptor does not exist\n");
		abort();
	}

	fseek(fp, 0L, SEEK_END);
	u64 len = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	return len;
}

u64 file_get_len_from_name(char * filename) {
	FILE * fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "file_get_len_from_name(): file %s can't be read\n", filename);
		abort();
	}

	u64 len = file_get_len(fp);
	fclose(fp);
	return len;
}

file_t * file_load(char * filename) {
	FILE * fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "file_load(): file %s can't be read\n", filename);
		abort();
	}

	file_t * file = new(sizeof(file));
	file->len = file_get_len(fp);
	file->buffer = new(file->len);

	if (fread(file->buffer, file->len, 1, fp) != 1) {
		fprintf(stderr, "file_load(): error while reading file %s of length %llu\n", filename, file->len);
		abort();
	}

	return file;
}

char * file_read_str(char * filename) {
	FILE * fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "file_read_str(): file %s can't be read\n", filename);
		abort();
	}

	u64 len = file_get_len(fp);
	char * buffer = new(len + 1);
	
	if (fread(buffer, len, 1, fp) != 1) {
		fprintf(stderr, "file_read_str(): error while reading file %s of length %llu\n", filename, len);
		abort();
	}

	buffer[len] = '\0';

	return buffer;
}
