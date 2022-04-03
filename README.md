# slang
### a compiled, statically-typed programming language just for fun

```cpp
// hello world

main(argc: int, argv: []str): int 
{
    print "Hello, World!\n"
    
    return 0
}
```
```cpp
// fizzbuzz

main(argc: int, argv: []str): int 
{
    @ i: int = 1, i < 20, i++
    {
        ? i % 3 == 0 {
            print "Fizz"
        } | i % 5 == 0 {
            print "Buzz"
        } | i % 3 == 0 & i % 5 == 0 {
            print "FizzBuzz"
        } | {
            print i
        }
    }
    
    return 0
}
```

Using the RBX register on my computer throws a segmentation fault. Not sure why.