# slang
### a compiled, statically-typed programming language with easy to type syntax and high performance

```cpp
// hello.sl

main
{
    dumpln "Hello, World!"
}
```
```cpp
// fizzbuzz.sl

main
{
    @ i: int = 1, i < 20, i++
    {
        ? i % 3 == 0 {
            dumpln "Fizz"
        } | i % 5 == 0 {
            dumpln "Buzz"
        } | i % 3 == 0 && i % 5 == 0 {
            dumpln "FizzBuzz"
        } | {
            dumpln i
        }
    }
}
```

## Language Features

### Checklist
- [x] Compile to C
- [x] Recursive and non-recursive functions
- [x] Custom types
- [ ] Type-checking
- [ ] Auto memory handling
- [ ] Compile to assembly

### Fixed-size Datatypes
| Keyword   | Description         | Size         |
|-----------|---------------------|--------------|
| `bool`      | boolean (`tru`/`fls`)   | 1 byte       |
| `char`      | character           | 1 byte       |
| `int`       | integer             | 4 bytes      |
| `uint`      | unsigned integer    | 4 bytes      |

### Dynamic-size Datatypes
| Keyword   | Description         |
|-----------|---------------------|
| `[char]`    | string              |
| `[type]`    | array               |

### Built-in generic functions
- `dump v`: dump a value v to the console. Takes in any fixed-size datatype or string and dumps the output to the console without a trailing new line

- `dumpln v`: dump a value v to the console with a new line. Takes in any fixed-size datatype or string and dumps the output to the console with a trailing new line

### String functions
- `insert s: string i: uint v: char/string`: inserts `v` into `s` at index `i` 
- `pushb s: string v: char/string`: pushes `v` at the back of `s`
- `pushf s: string v: char/string`: pushes `v` at the front of `s`
- `remove s: string i: uint [n]: uint`: removes `n` characters from `s` starting at index `i`. `n` is optional; default value is 1
- `popb s: string`: removes the last character from `s`
- `popf s: string`: removes the first character from `s`
- `(size s: string): uint`: returns the number of characters in `s`

### String operations
- `(s: string + v: char/string)`: returns a new string with all characters in `s` followed by all characters in `v`
- `s: string += v: char/string`: appends all characters in `v` to `s`
- `s == v`: returns `tru` all characters in `s` match all characters in `v`
- `s != v`: returns `tru` if a character in `s` does not match a character in `v`