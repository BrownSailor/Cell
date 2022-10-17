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