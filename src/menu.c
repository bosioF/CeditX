#include <stdio.h>

void show_menu() {
    printf("\n=== CeditX ===\n");
    printf("Command: ");
}

void help_menu() {
    printf("\n1. Load file (load <filename>)\n");
    printf("2. Save file (save [filename])\n");
    printf("3. View (view [offset] [lines])\n");
    printf("4. Edit byte (edit <offset> <hex_value>)\n");
    printf("5. Search pattern (search <hex_string>)\n");
    printf("6. Info (info)\n");
    printf("7. Exit (exit)\n");
}