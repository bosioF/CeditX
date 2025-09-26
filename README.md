# CeditX, a simple and lightweight hex editor made in C

A simple but complete hex editor written in C for educational purposes.

## Features

- **Hexadecimal visualization** with ASCII representation
- **Loading and saving** binary files
- **Direct byte editing**
- **Hexadecimal pattern search**
- **Intuitive command-line interface**
- **Safe memory management** with bounds checking
- **Support for files up to 1MB**

## Requirements

- C compiler (GCC, Clang, MSVC)
- Operating system: Linux, macOS, Windows
- Standard C libraries

## Installation

```bash
# Clone the repository
git clone https://github.com/bosioF/CeditX.git
cd CeditX/src

# Compile the program
gcc -o CeditX main.c -Wall -Wextra

# Run
./hexedit
````

---

## Compilation on Windows

### Option 1: MinGW (recommended)

1. Download and install [MinGW-w64](https://www.mingw-w64.org/).
2. Add `gcc` to your **PATH** during installation.
3. Open **Command Prompt** or **PowerShell**.
4. Navigate to the project folder:

   ```cmd
   cd C:\Users\YourName\CeditX\src
   ```
5. Compile the program:

   ```cmd
   gcc -o hexedit.exe main.c -Wall -Wextra
   ```
6. Run:

   ```cmd
   hexedit.exe
   ```

### Option 2: Microsoft Visual Studio (MSVC)

1. Install **Visual Studio Community**.
2. Select **Desktop development with C++** in the installer.
3. Open **x64 Native Tools Command Prompt for VS**.
4. Compile with:

   ```cmd
   cl main.c
   ```

   → This will generate `main.exe`.

### Option 3: WSL (Windows Subsystem for Linux)

1. Enable **WSL** and install Ubuntu.
2. Inside Ubuntu, install GCC:

   ```bash
   sudo apt update
   sudo apt install build-essential
   ```
3. Compile and run:

   ```bash
   gcc -o hexedit main.c -Wall -Wextra
   ./hexedit
   ```

---

## Usage

### Available commands

| Command  | Syntax                      | Description                          |
| -------- | --------------------------- | ------------------------------------ |
| `load`   | `load <filename>`           | Load a binary file                   |
| `save`   | `save [filename]`           | Save file (optionally with new name) |
| `view`   | `view [offset] [lines]`     | Display content in hex format        |
| `edit`   | `edit <offset> <hex_value>` | Modify a specific byte               |
| `search` | `search <hex_pattern>`      | Search for a hexadecimal pattern     |
| `info`   | `info`                      | Show current file information        |
| `help`   | `help`                      | Show command menu                    |
| `exit`   | `exit` or `quit`            | Exit the program                     |

### Practical examples

```bash
# Start the program
./hexedit

# Load a file
load myfile.bin

# Display first 10 lines
view 0 10

# Display from byte 256 onwards
view 0x100 5

# Modify byte at offset 32 with value FF
edit 0x20 FF

# Search for pattern "DEADBEEF"
search DEADBEEF

# Save changes
save

# Save with new name
save modified_file.bin

# Exit program
exit
```

## Sample output

```
Offset   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  ASCII
-------- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  ----------------
00000000 48 65 6C 6C 6F 20 57 6F 72 6C 64 21 0A 00 FF AA  Hello World!....
00000010 DE AD BE EF CA FE BA BE 01 23 45 67 89 AB CD EF  ........#Eg.....
```

## Code structure

### Main files

* `main.c` - Main source code
* `README.md` - This documentation

### Key components

* **HexBuffer**: Data structure that manages the file in memory
* **File I/O**: Functions for binary loading and saving
* **Display**: Formatted visualization in hex + ASCII
* **Command Parser**: Interactive command interpreter
* **Memory Management**: Safe allocation and deallocation

### Main functions

```c
HexBuffer* hex_buffer_init(const char *filename);     // Initialization
int hex_buffer_load(HexBuffer *buffer, const char *filename);  // Loading
int hex_buffer_save(HexBuffer *buffer, const char *filename);  // Saving
void hex_display(HexBuffer *buffer, size_t offset, size_t lines); // Display
int hex_edit_byte(HexBuffer *buffer, size_t offset, unsigned char value); // Edit
void hex_search(HexBuffer *buffer, const char *hex_string); // Search
```

## Current limitations

* Maximum file size: 1MB (defined by `MAX_BUFFER_SIZE`)
* Complete loading in memory (no streaming)
* Text-only interface
* No undo/redo of modifications
* Simple search (no regex or complex patterns)

## Next improvements

* [ ] Support for large files (streaming)
* [ ] Graphical interface with ncurses
* [ ] Modification history (undo/redo)
* [ ] Advanced search with regex
* [ ] Multiple display modes (8/16/32 bit)
* [ ] Support for different encodings
* [ ] Plugin system for specific formats
