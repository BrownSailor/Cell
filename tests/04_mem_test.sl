// Test memory storing and loading

// store "abc" to memory
mem 0 + 97 .
mem 1 + 98 .
mem 2 + 99 .

// write syscall
// Format is: num_bytes buf fd sys_write
3 mem 1 sys_write

// change "abc" to "bdf" by incrementing char 1 by 1, char 2 by 2, etc.
mem 0 + mem 0 + , 1 + .
mem 1 + mem 1 + , 2 + .
mem 2 + mem 2 + , 3 + .

3 mem 1 sys_write