#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hexbuffer.h"

#define BYTES_PER_LINE 16
#define MAX_FILENAME 256
#define MAX_BUFFER_SIZE 1048576 // 1MB

unsigned char old_value = 0;

HexBuffer* hex_buffer_init(const char *filename) {
    HexBuffer *buffer = malloc(sizeof(HexBuffer));
    if (!buffer) return NULL;

    buffer->data = malloc(MAX_BUFFER_SIZE);
    if (!buffer->data) {
        free(buffer);
        return NULL;
    }

    buffer->size = 0;
    buffer->capacity = MAX_BUFFER_SIZE;
    buffer->modified = 0;

    strncpy(buffer->filename, filename ? filename : "", MAX_FILENAME - 1);
    buffer->filename[MAX_FILENAME - 1] = '\0';

    return buffer;
}

void hex_buffer_free(HexBuffer *buffer) {
    if (buffer) {
        free(buffer->data);
        free(buffer);
    }
}

int hex_buffer_load(HexBuffer *buffer, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening the file\n");
        return 0;
    }

    buffer->size = fread(buffer->data, 1, buffer->capacity, file);
    strncpy(buffer->filename, filename, MAX_FILENAME);
    buffer->filename[MAX_FILENAME - 1] = '\0';

    printf("File '%s' loaded: %zu bytes\n", filename, buffer->size);
    fclose(file);
    return 1;
}

int hex_buffer_save(HexBuffer *buffer, const char *filename) {
    const char *save_filename = filename ? filename : buffer->filename;

    if (save_filename == 0) {
        printf("Invalid filename\n");
        return 0;
    }

    FILE *file = fopen(save_filename, "wb");
    if (!file) {
        printf("Error: cannot save to '%s'\n", save_filename);
        return 0;
    }

    size_t written = fwrite(buffer->data, 1, buffer->size, file);
    fclose(file);

    if (written == buffer->size) {
        printf("Bytes successfully written to '%s'\n", save_filename);
        buffer->saved = 1;
        buffer->modified = 0;
        return 1;
    } else {
        printf("Error writing file\n");
        return 0;
    }
}

void hex_display(HexBuffer *buffer, size_t offset, size_t lines) {
    if (!buffer->data || buffer->size == 0) {
        printf("\nNo file loaded.\n");
        return;
    }

    size_t end = offset + (lines * BYTES_PER_LINE);
    if (end > buffer->size) end = buffer->size;

    printf("\nOffset  ");
    for (int i = 0; i < BYTES_PER_LINE; i++) {
        printf("%02X ", i);
    }

    printf(" ASCII\n");
    printf("-------- ");
    
    for (int i = 0; i < BYTES_PER_LINE; i++) {
        printf("-- ");
    }
    printf(" ----------------\n");

    for (size_t i = offset; i < end; i += BYTES_PER_LINE) {
        printf("%08zX ", i);

        for (int j = 0; j < BYTES_PER_LINE; j++) {
            if (i + j < buffer->size) {
                printf("%02X ", buffer->data[i + j]);
            } else {
                printf("    ");
            }
        }

        printf(" ");
        for (int j = 0; j < BYTES_PER_LINE && i + j < buffer->size; j++) {
            unsigned char c = buffer->data[i + j];
            printf("%c", isprint(c) ? c : '.');
        }
        printf("\n");
    }
    printf("\n");
}

int hex_edit_file(HexBuffer *buffer, size_t offset, unsigned char value) {
    if (offset >= buffer->size) {
        printf("Offset out of bounds! (size: %zu)\n", buffer->size);
        return 0;
    }

    old_value = buffer->data[offset];
    buffer->data[offset] = value;

    buffer->modified = 1;
    buffer->saved = 0;

    printf("Byte at offset %zu modified: 0x%02X -> 0x%02X\n", offset, old_value, value);
    return 1;
}

void hex_search(HexBuffer *buffer, const char *hex_string) {
    if (!buffer->data || buffer->size == 0) {
        printf("Buffer is empty\n.");
        return;
    }

    size_t pattern_len = strlen(hex_string) / 2;
    unsigned char *pattern = malloc(pattern_len);
    if (!pattern) return;

    for (size_t i = 0; i < pattern_len; i++) {
        sscanf(hex_string + i * 2, "%2hhx", &pattern[i]);
    }

    int found = 0;
    for (size_t i = 0; i <= buffer->size - pattern_len; i++) {
        if (memcmp(buffer->data + i, pattern, pattern_len) == 0) {
            printf("\nFound at offset: 0x%08zX (%zu)", i, i);
            found++;
        }
    }

    if (found == 0) {
        printf("Pattern not found!\n");
    } else {
        printf("\n%d occurrences found\n", found);
    }

    free(pattern);
}

void hex_info(HexBuffer *buffer) {
    if (!buffer->data || buffer->size == 0) {
        printf("\nNo file loaded.\n");
        return;
    } else {
        printf("\n=== File Information ===\n");
        printf("Filename: %s\n", strlen(buffer->filename) ? buffer->filename : "(none)");
        printf("Size: %zu bytes\n", buffer->size);
        printf("Modified: %s\n", buffer->modified ? "Yes" : "No");
        printf("========================\n\n");
    }
}