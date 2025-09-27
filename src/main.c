#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BYTES_PER_LINE 16
#define MAX_FILENAME 256
#define MAX_BUFFER_SIZE 1048576 // 1MB

unsigned char old_value = 0;
unsigned char new_value = 0;
size_t value_offset = 0;

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
    char filename[MAX_FILENAME];
    int modified;
    int saved;
} HexBuffer;

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
        printf("Buffer is empty.\n");
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
    new_value = buffer->data[offset];
    value_offset = offset;

    buffer->modified = 1;
    buffer->saved = 0;

    printf("Byte at offset %zu modified: 0x%02X -> 0x%02X\n", offset, old_value, value);
    return 1;
}

void hex_undo(HexBuffer *buffer) {
    if (value_offset == 0 && new_value == 0 && old_value == 0) {
        printf("\nNo operations to undo.\n");
    } else {
        buffer->data[value_offset] = old_value;
        printf("\nUndo: edit\n");
        printf("Byte at offset %zu modified: 0x%02X -> 0x%02X\n", value_offset, new_value, old_value);
    }   
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
    printf("\n=== File Information ===\n");
    printf("Filename: %s\n", strlen(buffer->filename) ? buffer->filename : "(none)");
    printf("Size: %zu bytes\n", buffer->size);
    printf("Modified: %s\n", buffer->modified ? "Yes" : "No");
    printf("========================\n\n");
}

void show_menu() {
    printf("\n=== CeditX ===\n");
    printf("Command: ");
}

void help_menu() {
    printf("\n1. Load file (load <filename>)\n");
    printf("2. Save file (save [filename])\n");
    printf("3. View (view [offset] [lines])\n");
    printf("4. Edit byte (edit <offset> <hex_value>) or (edit undo) to undo last edit.\n");
    printf("5. Search pattern (search <hex_string>)\n");
    printf("6. Info (info)\n");
    printf("7. Exit (exit)\n");
}

int main() {
    HexBuffer *buffer = hex_buffer_init(NULL);
    if (!buffer) {
        printf("Error: unable to initialize buffer\n");
        return 1;
    }

    char command[256];
    char arg1[256], arg2[256], arg3[256], arg4[256];

    printf("CeditX, hex editor in C\n");

    while (1) {
        show_menu();

        if (!fgets(command, sizeof(command), stdin)) break;
        
        command[strcspn(command, "\n")] = '\0';
        int args = sscanf(command, "%255s %255s %255s %255s", arg1, arg2, arg3, arg4);
        
        if (args == 0) {
            continue;
        }

        if (strcmp(arg1, "exit") == 0 || strcmp(arg1, "quit") == 0) {
            if (!(args > 1)) {
                if (buffer->modified && buffer->saved != 1) {
                    printf("Warning, file not saved. Save? (y/n): ");
                    char response;
                    if (scanf(" %c", &response) == 1 && (response == 'y' || response == 'Y')) {
                        hex_buffer_save(buffer, NULL);
                    }
                    while (getchar() != '\n');
                }
            break;
        } else {
            printf("\nError: too many argouments.\n");
        }
        } 
        else if (strcmp(arg1, "load") == 0) {
            if (!(args > 2)) {
            hex_buffer_load(buffer, arg2);
            }
            else {
                printf("\nError: too many argouments.\n");
            }
        }
        else if (strcmp(arg1, "save") == 0) {
            if (!(args >= 3)) {
            hex_buffer_save(buffer, args == 2 ? arg2 : NULL);
            }
            else {
                printf("\nError: too many argouments");
            }
        } 
        else if (strcmp(arg1, "view") == 0) {
            if (!(args > 1)) {
                size_t offset = args >= 2 ? strtoull(arg2, NULL, 0) : 0;
                size_t lines = args >= 3 ? strtoull(arg3, NULL, 0) : 20;
                hex_display(buffer, offset, lines);
            } else {
                printf("\nError: too many argouments");
            }
        }
        else if (strcmp(arg1, "edit") == 0) {
            if (args < 2) {
                printf("\nError: too few arguments\n");
            } else if (args == 2) {
                if (strcmp(arg2, "undo") == 0) {
                    hex_undo(buffer);
                } else {
                    printf("\nError: unknown subcommand '%s'\n", arg2);
                }
            } else if (args == 3) {
                size_t offset = strtoull(arg2, NULL, 0);
                unsigned char value = (unsigned char)strtoul(arg3, NULL, 16);
                hex_edit_file(buffer, offset, value);
            } else {
                printf("\nError: too many arguments.\n");
            }
        }
        else if (strcmp(arg1, "search") == 0 && args == 2) {
            hex_search(buffer, arg2);
        } 
        else if (strcmp(arg1, "info") == 0) {
            hex_info(buffer);
        }
        else if (strcmp(arg1, "help") == 0) {
            help_menu();
        }
        else {
            printf("Command not recognized. Use 'help' to see available commands.\n");
        }
    }

    hex_buffer_free(buffer);
    printf("\nGoodbye!\n");
    return 0;
}