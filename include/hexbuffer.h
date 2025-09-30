#ifndef HEXBUFFER_H
#define HEXBUFFER_H

#include <stddef.h>

#define MAX_FILENAME 256

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
    char filename[MAX_FILENAME];
    int modified;
    int saved;
} HexBuffer;

HexBuffer* hex_buffer_init(const char *filename);
void hex_buffer_free(HexBuffer *buffer);
int hex_buffer_load(HexBuffer *buffer, const char *filename);
int hex_buffer_save(HexBuffer *buffer, const char *filename);
void hex_display(HexBuffer *buffer, size_t offset, size_t lines);
int hex_edit_file(HexBuffer *buffer, size_t offset, unsigned char value);
void hex_search(HexBuffer *buffer, const char *hex_string);
void hex_info(HexBuffer *buffer);

#endif