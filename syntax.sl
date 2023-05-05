fn hello: nil -> nil {
    println "Hello, world!"
}

fn fizzbuzz: num -> nil {
    N

    i: 1
    @ i <= N {
        ? i % 3 = 0 and i % 5 = 0 {
            println "FizzBuzz"
        } | i % 3 = 0 {
            println "Fizz"
        } | i % 5 = 0 {
            println "Buzz"
        } | {
            println i
        }

        i: i + 1
    }
}

fn gcd: num num -> num {
    a b
    ? b = 0 { a } | { gcd (b a % b) }
}

fn divmod: num num -> num num {
    n k

    n / k
    n % k
}

table: 20 * num
table[1]: 34
