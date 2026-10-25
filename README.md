# SNHU_CS300_DSA-_Analysis_and_Design

## Reflection

### What was the problem you were solving in the projects for this course?

ABCU's Computer Science department needed an advising assistance program to help advisors look up course information. The program had to load course data from a file, print every Computer Science course in alphanumeric order and display any single course's title along with its prerequisites. Behind that request was the real problem: choosing a data structure that could store the courses and search them efficiently.

### How did you approach the problem? Consider why data structures are important to understand.

I started by writing pseudocode for three options (a vector, a hash table, and a binary search tree) and analyzed the Big O runtime of each for loading, searching, and printing the courses. That analysis drove my final choice. A vector is simple but has to be sorted before it can print in order. A hash table gives fast lookups but stores no order. A binary search tree keeps the courses sorted as they are inserted, so the in-order print comes almost for free while searches stay fast. This project showed me that the data structure is not just an implementation detail. It decides how well the program performs and how complex the code becomes.

### How did you overcome any roadblocks you encountered while going through the activities or project?

My biggest roadblocks were parsing the course file correctly and managing pointers in the tree. Some lines had no prerequisites and others had several, so I had to split each line carefully and validate that every prerequisite actually existed as a course in the file. For the tree, I traced insert and search operations on paper before coding them, which helped me catch null pointer mistakes early. I also tested with small sample files first so I could isolate one problem at a time instead of debugging everything at once.

### How has your work on this project expanded your approach to designing software and developing programs?

I now think about how data will be used before I decide how to store it. Writing pseudocode and comparing runtimes before coding saved me from committing to the wrong structure and rewriting later. I also break programs into clear steps (load, validate, store, search, display) and design each one separately, which makes the whole program easier to build and test.

### How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

I try to write smaller functions that each do one job, use descriptive names and add comments that explain why the code works the way it does, not just what it does. I also separated the data structure logic from the menu and file-handling code, so the storage method could be swapped without rewriting the rest of the program. Validating input and handling errors gracefully has become part of how I write code from the start, not something I add at the end.
