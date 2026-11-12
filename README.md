*This activity has been created as part of the 42 curriculum by basianno.*

# DESCRIPTION

The goal of this project is to write a function that returns a line read from a file descriptor.
The main point of the project is to learn how static variables work in C (variables that keep their value between two calls of a function).

Prototype:

```c
char	*get_next_line(int fd);
```

Each call to `get_next_line` returns the next line of the file, with its `\n` at the end (except if the file ends without one). It returns `NULL` when there is nothing left to read or if an error happens.
It works with a file and with the standard input.

Rules of the subject:

- only `read`, `malloc` and `free` are allowed as external functions
- the libft is not allowed, so the helper functions are recoded in `get_next_line_utils.c`
- `lseek` and global variables are forbidden
- the function must read as little as possible at each call, and not the whole file at once

Files: `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`.

# INSTRUCTIONS

There is no Makefile: the files are compiled directly with your program.

```
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c
```

`-D BUFFER_SIZE=n` sets the number of bytes read by each call to `read`. The project also compiles without this flag: the default value is then 42.

Include the header in your source files: `#include "get_next_line.h"`.

The returned line is allocated with `malloc`: you have to `free` it after use.

Example:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

To read from the standard input, call `get_next_line(0)`.

# RESOURCES

Man pages (`man 2 read`, `man 2 open`, `man 3 malloc`), Stack Overflow, GitHub, YouTube tutorials.

I used AI as a teacher: it explained notions I did not know (static variables, file descriptors, how `read` moves in a file), guided me when needed.
AI also helped me to review my code to find bugs or leaks and with testing programs (different `BUFFER_SIZE` values, empty files, files without a final newline).
All the code of the project was written by me.

# DETAILED DESCRIPTION OF THE PROJECT

## Functions

`get_next_line` = reads the file until a line is complete, returns it and keeps the rest for the next call\
`check_verif` = checks the arguments, allocates the read buffer and creates the stash on the first call\
`stash_extraction` = returns a copy of the stash up to the first `\n` (included)\
`stash_update` = keeps only what comes after the first `\n` in the stash\
`ft_free` = frees the buffer and the stash when an error happens

`ft_strlen` = returns the length of a string\
`ft_strchr` = finds the first occurrence of a character in a string\
`ft_strdup` = returns an allocated copy of a string\
`ft_strjoin` = returns the concatenation of two strings

## Algorithm and data structure

**The stash.** `read` gives `BUFFER_SIZE` bytes at a time, so it often reads more than one line, or only a part of a line. The characters that were read but not returned yet are kept in a static string called `stash`. Because it is static, it is still there at the next call.

**Reading.** While the stash has no `\n` and `read` does not return 0, the function reads `BUFFER_SIZE` bytes into a buffer, adds a `\0` at the end, and joins the buffer to the stash with `ft_strjoin`. The old stash is freed after each join.

**Returning the line.** When the loop stops, the stash holds at least one full line (or the end of the file). `stash_extraction` copies everything up to the first `\n` into a new string: this is the returned line. Then `stash_update` replaces the stash with what comes after the `\n`, so the next call starts from there.

**End of file.** When `read` returns 0, the loop stops. What is left in the stash is returned as the last line, and the stash is freed and set to `NULL`. At the next call the stash is empty, so the function returns `NULL`.

**Why this design.** The subject asks to read as little as possible: the loop stops as soon as the stash contains a `\n`, so the function never reads the whole file before returning a line. `lseek` is forbidden, so it is not possible to go back in the file: the characters read after the `\n` must be kept somewhere, and global variables are forbidden too, so a static variable is the only way to keep them between two calls. A string is enough to store this leftover, so there is no linked list or structure to manage. Each helper has one job, every allocation has one place where it is freed.

## Technical choices for special cases

`fd < 0` or `BUFFER_SIZE <= 0` = returns `NULL` without reading\
`read` error = returns `NULL`, the buffer and the stash are freed and the stash is set to `NULL`\
`malloc` error = returns `NULL`, everything already allocated is freed\
Empty file = returns `NULL` at the first call\
Last line without `\n` = returned as it is, without adding a `\n`\
Very large `BUFFER_SIZE` (example: 10000000) = the buffer is allocated with `malloc` and not on the stack, so it does not overflow the stack\
`BUFFER_SIZE` of 1 = works, the file is read one byte at a time\
Very long line with a small `BUFFER_SIZE` = works, but slowly, because the whole stash is copied at each `read`\
Several file descriptors at the same time = not handled, there is only one stash and the subject does not ask for it\
Binary file, or file modified between two calls = undefined behavior, as written in the subject
