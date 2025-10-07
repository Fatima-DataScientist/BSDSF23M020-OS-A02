Feature 7: ls-v1.6.0 – Recursive Listing (-R)

1. In a recursive function, what is a "base case"? In the context of your recursive ls, what is the base case that stops the recursion from continuing forever?

Answer:
A base case in recursion is the condition under which the recursive function stops calling itself to prevent infinite recursion. 
In the recursive ls implementation, the base case occurs when a directory contains no subdirectories (or when all
