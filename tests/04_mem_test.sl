// Test memory storing and loading

// store "abc" to memory
mem 0 + 97 store
mem 1 + 98 store
mem 2 + 99 store

// write syscall
// Format is: num_bytes buf fd sys_write
3 mem 1 sys_write

// change "abc" to "bdf" by incrementing char 1 by 1, char 2 by 2, etc.
mem 0 + mem 0 + load 1 + store
mem 1 + mem 1 + load 2 + store
mem 2 + mem 2 + load 3 + store

3 mem 1 sys_write