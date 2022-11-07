main {
    @ i: int = 1, i <= 20, i = i + 1 {
        ? i % 15 == 0 {
            dumpln "FizzBuzz"
        } | i % 3 == 0 {
            dumpln "Fizz"
        } | i % 5 == 0 {
            dumpln "Buzz"
        } | {
            dumpln i
        }
    }
}
