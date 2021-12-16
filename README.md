# Cell

An experiment to create a compiled stack-based programming language just for fun

## Example
```
2 3 + .
```

will produce console output 

```
5
```

## Usage

### Simulation

Simulation runs an interpreter on the program

```js
$ cat program.cll

2 3 + .

$ ./cell sim program.cll

5
```

### Compilation

Compilation generates assembly code, compiles it with [nasm](https://www.nasm.us/), and links it with [ld](https://www.gnu.org/software/binutils/)

```js
$ cat program.cll

2 3 + .

$ ./cell com program.out program.cll

[INFO] Generating program.asm

[CMD] nasm -o program.o program.asm

[CMD] ld program.o -o program.out

$ ./program.out

5
```
