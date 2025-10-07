1. ANSI Escape Codes for Color in Linux Terminal:
ANSI escape codes are special sequences of characters that the terminal interprets to apply text formatting such as colors, bold, underline, or background changes. They begin with the ESC character (ASCII 27) followed by `[` and end with a code specifying the format, then `m`. After printing the text, you reset the formatting using the reset code.

Example: To print text in green:
printf("\033[0;32m%s\033[0m\n", "This text is green");

Explanation:
- \033: Escape character (ESC)
- [0;32m: 0 = default style, 32 = green foreground
- \033[0m: Reset formatting to default after the text

2. Checking Executable Permission Bits in st_mode:
The st_mode field of struct stat contains both file type and permission bits. To determine if a file is executable:
- Owner executable: S_IXUSR (0400)
- Group executable: S_IXGRP (0040)
- Others executable: S_IXOTH (0004)

You can check if a file is executable by using bitwise AND with st_mode:
if (st.st_mode & S_IXUSR) { /* Owner can execute */ }
if (st.st_mode & S_IXGRP) { /* Group can execute */ }
if (st.st_mode & S_IXOTH) { /* Others can execute */ }
