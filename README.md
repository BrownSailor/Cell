# slang
### a compiled programming language with easy to type syntax and high performance

## Syntax
### Built-in types
- `num`: a 64 bit integer
- `bool`: a boolean (tru/fls)
- `str`: a string

```
; hello.sl
fn hello: nil -> nil {
    println "Hello, world!"
}
```

```
; fizzubuzz.sl
fn fizzbuzz: num -> nil {
    n

    i: 1
    @ i <= n {
        ? i % 3 = 0 and i % 5 = 0 {
            print "FizzBuzz"
        } | i % 3 = 0 {
            print "Fizz"
        } | i % 5 = 0 {
            print "Buzz"
        } | {
            print i
        }

        i: i + 1
    }
}
```
