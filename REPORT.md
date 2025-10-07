1. Why is it necessary to read all directory entries into memory before you can sort them? 
   What are the potential drawbacks of this approach for directories containing millions of files?

Answer:
Reading all directory entries into memory is necessary because sorting requires access to all filenames at once. Functions like qsort() need an array of all elements to compare and rearrange them. The main drawback of this approach is memory usage: for directories with millions of files, storing all names in memory can consume a large amount of RAM, potentially leading to slow performance or memory exhaustion.

2. Explain the purpose and signature of the comparison function required by qsort(). How does it work, and why must it take const void * arguments?

Answer:
The comparison function provides qsort() with the rules for ordering elements. Its signature is:
    int cmp(const void *a, const void *b);

It takes two const void * pointers to allow qsort() to work with arrays of any type. Inside the function, these pointers are cast to the actual data type (in our case, char **) and then compared using strcmp(). The function returns a negative value if the first element is smaller, zero if equal, and positive if larger, guiding qsort() in arranging the array in ascending alphabetical order.
