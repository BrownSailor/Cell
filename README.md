# slang
### a compiled programming language with easy to type syntax and high performance

## Syntax
### Built-in types
- `num`: a 64 bit integer
- `bool`: a boolean (tru/fls)
- `str`: a string

```
; hello.sl
fn hello {
    println "Hello, world!"
}
```

```
; fizzubuzz.sl
fn fizzbuzz: num {
    in >> n

    @ i in 1 to n by 1 {
        ? i % 3 = 0 and i % 5 = 0 {
            print "FizzBuzz"
        } | i % 3 = 0 {
            print "Fizz"
        } | i % 5 = 0 {
            print "Buzz"
        } | {
            print i
        }
    }
}
```
