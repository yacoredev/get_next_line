*This project has been created as part of the 42 curriculum by yabaadi.*

# Get Next Line

## Description

Get Next Line (GNL) is a 42 School project that consists of implementing a function capable of reading a file descriptor one line at a time.

The objective of this project is to understand how file descriptors, the `read()` system call, static variables, and dynamic memory allocation work together.

The function must return one line per call, including the newline character (`\n`) if it exists. When there is nothing left to read or an error occurs, it must return `NULL`.

The project also introduces the concept of persistent data between function calls, allowing the function to preserve unread data and continue reading from where it stopped.

Get Next Line must work with regular files as well as standard input (`stdin`).

### Function Prototype

```c
char *get_next_line(int fd);
```

### Main Objectives

- Read from a file descriptor using `read()`.
- Return one line per function call.
- Preserve unread data between calls.
- Manage dynamic memory correctly.
- Handle different `BUFFER_SIZE` values.
- Support standard input.
- Handle EOF and reading errors.
- In the bonus part, manage multiple file descriptors simultaneously.

---

## Algorithm

### 1. Mandatory Part

The mandatory implementation uses a static pointer named `rest` to preserve data between function calls.

The algorithm is based on reading chunks of data, appending them to the stored string, and extracting one line whenever a newline is found.

**Step 1: Initialize the buffer**

A temporary buffer of `BUFFER_SIZE + 1` bytes is allocated. The extra byte is used to add the null terminator after the data returned by `read()`.

**Step 2: Read from the file descriptor**

The `get_line()` helper function repeatedly calls `read()` to obtain data from the specified file descriptor.

Each successful read is appended to the static `rest` string using `ft_strjoin()`.

**Step 3: Search for a newline**

After appending the newly read data, the function searches for `\n` inside `rest`.

- If no newline is found, reading continues.
- If a newline is found, the reading loop stops.
- If EOF is reached, the function processes any remaining data.

**Step 4: Extract the current line**

The `extract_line()` helper function searches for the first newline.

If a newline exists, it extracts the string from the beginning up to and including `\n`.

If there is no newline, it returns the remaining content as the final line.

**Step 5: Update the remaining data**

The `update_rest()` helper function removes the returned line from `rest`.

The characters following the newline are copied into a new string and stored in `rest` for the next function call.

If no remaining data exists, `rest` is freed and set to `NULL`.

**Step 6: Return the line**

The extracted line is returned to the caller, while the remaining data stays stored for the next call.

### Mandatory Algorithm Example

Consider a file containing:

```text
Hello
World
42
```

Suppose one call to `read()` returns:

```text
Hello
World
```

After finding the first newline:

| Operation | Result |
|---|---|
| `rest` before extraction | `Hello\nWorld\n` |
| Returned line | `Hello\n` |
| `rest` after update | `World\n` |

On the next call, the function starts with the remaining content already stored in `rest`.

It does not need to read that data again.

This process continues until all lines have been returned.

### Why This Algorithm?

A static variable is necessary because local variables normally lose their values when a function returns.

Using a static pointer allows the function to preserve unread data between consecutive calls.

The implementation also avoids reading the entire file at once. It reads only until a complete line is available, then returns that line immediately.

This approach works with different buffer sizes because the algorithm does not assume that one call to `read()` corresponds to one line.

---

## Bonus Part

### Multiple File Descriptors

The bonus part extends the mandatory implementation to support multiple file descriptors simultaneously.

The function must be able to read from different file descriptors without mixing their contents.

For example, if two files are opened:

- File A contains `Hello\nWorld\n`.
- File B contains `42\n1337\n`.

The function should be able to alternate between both file descriptors while preserving the correct reading position and remaining data for each one.

### Bonus Algorithm

Instead of using a single static pointer, the bonus implementation uses a static array of character pointers:

```c
static char *rest[OPEN_MAX];
```

Each index represents the stored data associated with a file descriptor.

For example:

```text
rest[fd1] -> Remaining data from file A
rest[fd2] -> Remaining data from file B
rest[fd3] -> Remaining data from file C
```

The algorithm works as follows:

1. Validate the file descriptor.
2. Use `rest[fd]` to access the stored data associated with that descriptor.
3. Read and append data using the same logic as the mandatory implementation.
4. Search for the first newline.
5. Extract and return the current line.
6. Update only `rest[fd]`, keeping the remaining data for that specific descriptor.
7. Leave the stored data of all other file descriptors unchanged.

### Bonus Example

Imagine alternating calls between two files:

| Function call | File descriptor | Returned line |
|---|---|---|
| First call | `fd1` | `Hello\n` |
| Second call | `fd2` | `42\n` |
| Third call | `fd1` | `World\n` |
| Fourth call | `fd2` | `1337\n` |

Each file descriptor maintains its own remaining data independently.

### Why Use an Array?

An array allows each file descriptor to have its own persistent storage while keeping the implementation simple.

It also avoids using global variables and keeps the same general reading, extraction, and updating logic as the mandatory part.

The bonus implementation reuses the same algorithmic approach, with the main difference being that the stored data is indexed by file descriptor instead of using a single pointer.

---

## Technical Choices

### Static Variables

Static variables preserve their values between function calls.

In the mandatory implementation, a static pointer stores the unread content.

In the bonus implementation, a static array of pointers maintains separate unread content for each file descriptor.

### Dynamic Memory Allocation

Dynamic allocation is necessary because the amount of data read is not known in advance.

The implementation uses:

- `malloc()` to allocate buffers and strings.
- `free()` to release memory when it is no longer needed.

Every allocated string must be handled carefully to avoid memory leaks and invalid memory accesses.

### String Manipulation

Since Libft is forbidden in this project, the required string operations are implemented using helper functions.

These include:

- `ft_strlen()` — calculates string length.
- `ft_strjoin()` — concatenates two strings.
- `ft_strdup()` — duplicates a string.
- `ft_substr()` — extracts a substring.

---

## Project Structure

### Mandatory Files

```text
.
├── get_next_line.c
├── get_next_line_utils.c
├── get_next_line.h
└── README.md
```

**get_next_line.c**

Contains the main function and its internal helpers:

- `get_next_line()` — manages the reading process and returns the next line.
- `get_line()` — reads and accumulates data until a newline or EOF.
- `extract_line()` — extracts the current line.
- `update_rest()` — preserves the remaining unread content.
- `free_rest()` — releases the stored content.

**get_next_line_utils.c**

Contains the string manipulation helper functions used by the main implementation.

**get_next_line.h**

Contains the function prototype, required headers, and the default `BUFFER_SIZE` definition.

### Bonus Files

```text
.
├── get_next_line_bonus.c
├── get_next_line_utils_bonus.c
└── get_next_line_bonus.h
```

The bonus files implement the multiple-file-descriptor functionality.

The bonus implementation uses a static array to maintain separate data for each file descriptor.

---

## Instructions

### Compilation

The project can be compiled using the following command:

**Mandatory:**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
get_next_line.c get_next_line_utils.c main.c
```

**Bonus:**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

The project must also compile without explicitly defining `BUFFER_SIZE`, using the default value specified in the header file.

### Running

To test the function, create a `main.c` file and open a file using `open()`.

Example:

```c
int fd;
char *line;

fd = open("test.txt", O_RDONLY);
if (fd < 0)
    return (1);

while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}
close(fd);
```

For standard input, use file descriptor `0`.

---

## Testing

The implementation should be tested with different `BUFFER_SIZE` values and different file contents.

### Buffer Size Tests

```bash
-D BUFFER_SIZE=1
-D BUFFER_SIZE=42
-D BUFFER_SIZE=9999
-D BUFFER_SIZE=10000000
```

### Edge Cases

- Empty files.
- Files containing a single line.
- Files containing multiple lines.
- Files ending with `\n`.
- Files without a final `\n`.
- Very long lines.
- Invalid file descriptors.
- Different file descriptors used alternately (bonus).
- Standard input.
- EOF and reading errors.
- Memory allocation failures.

---

## Restrictions and Limitations

The project follows the restrictions defined in the 42 subject:

- Libft is forbidden.
- `lseek()` is forbidden.
- Global variables are forbidden.
- Only the permitted external functions may be used: `read()`, `malloc()`, and `free()`.
- The returned line must include `\n` when present.
- Reading binary files results in undefined behavior.
- Modifying the file after the last call while `read()` has not yet reached EOF results in undefined behavior.

---

## Resources

The following resources were useful for understanding the concepts involved in this project:

- [Linux Manual – read(2)](https://man7.org/linux/man-pages/man2/read.2.html) — Understanding how the `read()` system call works.
- [Linux Manual – malloc(3)](https://man7.org/linux/man-pages/man3/malloc.3.html) — Dynamic memory allocation.
- [Linux Manual – free(3)](https://man7.org/linux/man-pages/man3/free.3.html) — Releasing allocated memory.
- [cppreference – C Storage Duration](https://en.cppreference.com/w/c/language/storage_duration) — Understanding static and automatic storage duration.
- 42 Get Next Line subject — Project requirements and restrictions.

### AI Usage

AI was used as a learning and clarification tool throughout the project.

It helped with:

- Understanding C concepts such as static variables, pointers, file descriptors, and dynamic memory allocation.
- Understanding the behavior of `read()`, EOF, and different `BUFFER_SIZE` values.
- Clarifying the project requirements and identifying important edge cases.
- Discussing algorithmic approaches for preserving unread data.
- Understanding memory management and debugging potential issues.

The implementation, code structure, algorithm selection, testing, and debugging were carried out by the project student. AI assistance was limited to explanations, conceptual guidance, and clarification of technical questions.