#ifndef FILEIO_H
#define FILEIO_H

#include <types.h>
#include <stdio.h>

typedef struct file_t {
	u8 * buffer;
	u64 len;
} file_t;

u64 file_get_len(FILE * fp);
u64 file_get_len_from_name(char * filename);
file_t * file_load(char * filename);
char * file_read_str(char * filename);

#endif
