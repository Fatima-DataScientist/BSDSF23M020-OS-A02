# REPORT - Column Display Feature (v1.2.0)

## 1. General Logic for "Down Then Across" Columnar Printing

The "down then across" printing method prints filenames row by row across multiple columns. The steps involved are:

1. Read all directory entries into a dynamically allocated array of strings.
2. Keep track of the length of the longest filename.
3. Calculate the number of columns that can fit in the terminal:
   - Terminal width / (maximum filename length + spacing)
4. Calculate the number of rows needed:
   - rows = ceil(total_files / num_columns)
5. Iterate row by row and print each file from every column for that row:
   - For row i: print filenames[i], filenames[i + num_rows], filenames[i + 2*num_rows], ...
6. Pad each filename with spaces to ensure columns align properly.

**Why a simple loop is insufficient:**  
A single loop would print filenames linearly, producing a single column or a poorly aligned horizontal layout. The "down then across" format ensures that columns are evenly filled and visually aligned, making the output more readable.

---

## 2. Purpose of ioctl System Call

The `ioctl` system call with the `TIOCGWINSZ` request is used to programmatically detect the current terminal width. This allows the program to dynamically adjust the number of columns to fit the terminal screen.

**Limitations of a fixed-width fallback (e.g., 80 columns):**

- Columns may not fully utilize the terminal width or may exceed it, causing line wrapping.
- The output layout will not adapt when the terminal is resized.
- Overall user experience is less flexible and visually inconsistent across different terminal sizes.
