# Slang

An experiment to create a compiled stack-based programming language just for fun

## Example
```
2 3 + dump
```

will produce console output 

```
5
```

## Usage

### Compilation

Compilation generates assembly code, compiles it with [nasm](https://www.nasm.us/), and statically links it with [ld](https://www.gnu.org/software/binutils/)

```js
$ cat program.sl

2 3 + dump

$ ./slang program program.sl

[INFO] Generating program.asm

[CMD] nasm -o program.o program.asm

[CMD] ld program.o -o program.out

$ ./program

5
```

## Features

### Integers

Typing an integer automatically pushes it to the top of the stack.

```js
2 3 +
```
### Strings

Typingn a string automatically pushes two things to the stack: the string size
and the pointer to the string itself, in that order. That is, the pointer to
the string is at the top of the stack and the size of the string is underneath.

```js
"Hello, World!\n"
```

This allows us to call syscalls for write to print the string:

```js
"Hello, World!\n" stdout write

$ Hello, World!
```

### Arithmetic Operations
| Name      | Syntax     | Result      | Description     |
| --------- | ---------- | ----------- | --------------- |
| `+`       | `5 3 +`    | `8`      | Addition        |
| `-`       | `5 3 -`    | `2`      | Subtraction     |
| `*`       | `5 3 *`    | `15`      | Multiplication  |
| `/`       | `5 3 /`    | `1`      | Division        |
| `%`       | `5 3 %`    | `2`      | Modulo          |

### Boolean Operations
| Name      | Syntax     | Result      | Description     |
| --------- | ---------- | ----------- | --------------- |
| `=`      | `5 3 =`   | `0 (false)`      | Equality        |
| `!=`     | `5 3 !=`  | `1 (true)`      | Inequality      |
| `<`      | `5 3 <`   | `0 (false)`      | Less than       |
| `>`      | `5 3 >`   | `1 (true)`      | Greater than    |
| `<=`     | `5 3 <=`  | `0 (false)`      | Less than or equal to |
| `>=`     | `5 3 >=`  | `1 (true)`      | Greater than or equal to |

### Bitwise Operations
| Name      | Syntax     | Result     | Description     |
| --------- | ---------- | ----------- | --------------- |
| `and`       | `1 0 and`    | `0`      | Bitwise AND     |
| `or`        | `1 0 or`     | `1`      | Bitwise OR      |
| `xor`       | `1 0 xor`    | `1`      | Bitwise XOR     |
| `not`       | `1 not`      | `0`      | Bitwise NOT     |
| `shl`        | `1 shl`       | `2`      | Bitwise left shift |
| `shr`        | `1 shr`       | `0`      | Bitwise right shift |

### Stack Operations
| Name      | Syntax     | Result      | Description     |
| --------- | ---------- | ----------- | --------------- |
| `dup`       | `5 dup`    | `5 5`      | Duplicate top of stack |
| `drop`      | `5 3 drop`   | `5`      | Drop top of stack |
| `swap`      | `5 3 swap`   | `3 5`      | Swap top of stack |
| `over`      | `5 3 over`   | `5 3 5`      | Over top of stack |

### Memory Operations
| Name      | Syntax     | Description     |
| --------- | ---------- | --------------- |
| `mem`       | `mem name size end`    | Assign a fixed size block of memory with a name |
| `!8`       | `!8`      | Store a byte (8 bits) to memory |
| `@8`       | `@8`      | Load a byte (8 bits) from memory |
| `!16`       | `!16`      | Store a short (16 bits) to memory |
| `@16`       | `@16`      | Load a short (16 bits) from memory |
| `!32`       | `!32`      | Store an integer (32 bits) to memory |
| `@32`       | `@32`      | Load an integer (32 bits) from memory |
| `!64`       | `!64`      | Store a long (64 bits) to memory |
| `@64`       | `@64`      | Load a long (64 bits) from memory |
