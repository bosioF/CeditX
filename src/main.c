#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hexbuffer.h"
#include "menu.h"

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

        if (args == 0) continue;

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
                printf("\nError: too many arguments.\n");
            }
        }
        else if (strcmp(arg1, "load") == 0) {
            if (!(args > 2)) {
                hex_buffer_load(buffer, arg2);
            } else {
                printf("\nError: too many arguments.\n");
            }
        }
        else if (strcmp(arg1, "save") == 0) {
            if (!(args >= 3)) {
                hex_buffer_save(buffer, args == 2 ? arg2 : NULL);
            } else {
                printf("\nError: too many arguments\n");
            }
        }
        else if (strcmp(arg1, "view") == 0) {
            size_t offset = args >= 2 ? strtoull(arg2, NULL, 0) : 0;
            size_t lines = args >= 3 ? strtoull(arg3, NULL, 0) : 20;
            hex_display(buffer, offset, lines);
        }
        else if (strcmp(arg1, "edit") == 0) {
            if (args == 3) {
                size_t offset = strtoull(arg2, NULL, 0);
                unsigned char value = (unsigned char)strtoul(arg3, NULL, 16);
                hex_edit_file(buffer, offset, value);
            } else if (args > 3) {
                printf("\nError: too many argouments.\n");
            } else if (args < 3) {
                printf("\nError: too few argouments.\n");
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
            printf("Command not recognized. Use 'help'.\n");
        }
    }

    hex_buffer_free(buffer);
    printf("\nGoodbye!\n");
    return 0;
}
