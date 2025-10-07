# Custom `ls` Utility in C

This repository contains a custom implementation of the Linux `ls` command in C, developed incrementally from version 1.0.0 to 1.6.0. The project emphasizes file system traversal, command-line argument parsing, dynamic memory management, output formatting, and advanced features like recursive listing and colorized output.

---

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Version History](#version-history)
- [Contributing](#contributing)
- [License](#license)

---

## Features

### v1.0.0 – Basic Column Display
- Displays directory contents in vertical columns (down then across).
- Handles basic file listing for the current directory.

### v1.1.0 – Long Listing (`-l`)
- Supports detailed information for each file:
  - File type and permissions
  - Number of hard links
  - Owner and group
  - File size
  - Last modification time
- Mimics standard `ls -l` behavior.

### v1.3.0 – Horizontal Column Display (`-x`)
- Adds horizontal (row-major) column layout.
- Prints files left-to-right, wrapping based on terminal width.
- Provides a contrast to the default vertical layout.

### v1.4.0 – Alphabetical Sort
- Directory contents are sorted alphabetically before display.
- Sorting applies to all display modes: vertical (default), horizontal (`-x`), and long listing (`-l`).

### v1.5.0 – Colorized Output
- Adds color based on file type using ANSI escape codes:
  - **Directories:** Blue
  - **Executables:** Green
  - **Archives (.tar, .gz, .zip):** Red
  - **Symbolic Links:** Pink
  - **Special files (device files, sockets, FIFOs):** Reverse video
- Enhances readability and user experience.

### v1.6.0 – Recursive Listing (`-R`)
- Lists all files recursively in subdirectories.
- Prints directory headers before listing contents.
- Supports all display modes (`-l`, `-x`, default) and colorized output.

---

## Installation

1. Clone the repository:

```bash
git clone https://github.com/<username>/custom-ls.git
cd custom-ls
