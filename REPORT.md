1. Comparison of Implementation Complexity:

The "down then across" (vertical) printing logic is more complex than the "across" (horizontal) printing logic. 

- Vertical (down then across) mode requires pre-calculation of the number of rows and columns. This is because filenames need to be distributed across columns so that they are printed top-to-bottom first, then left-to-right. The program must know the total number of files, the maximum filename length, and the terminal width to calculate how many rows and columns can fit. It also needs to iterate over the filenames in a row-major order, which requires careful indexing.

- Horizontal (across) mode is simpler. Files are printed left-to-right in order, wrapping to the next line when the current line is full. It still uses the terminal width and maximum filename length for spacing, but no pre-calculation of rows is needed. A single loop tracking the current horizontal position is sufficient.

Therefore, the vertical printing logic requires more pre-calculation because it must determine the layout in terms of rows and columns before printing.

2. Strategy for Managing Display Modes:

In the code, a simple integer flag variable (or enum) is used to track the selected display mode:

- Default mode (0): Down then across columns
- Long-listing mode (-l): Detailed listing
- Horizontal mode (-x): Row-major horizontal display

The program uses `getopt()` to parse command-line arguments. When the `-l` option is detected, the flag is set to long-listing mode. When the `-x` option is detected, the flag is set to horizontal mode. After reading all filenames into an array, the main display function checks the flag and calls the appropriate printing function based on the selected mode. This allows all three display modes to coexist cleanly in the same program.
