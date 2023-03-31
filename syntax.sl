fn hello nil: -> nil {
    println "Hello, World!"
}

fn fizzbuzz: num -> nil {
    in >> N

    @ i in 1 to N by 1 {
        ? i % 3 == 0 and i % 5 == 0 {
            println "FizzBuzz"
        } | i % 3 == 0 {
            println "Fizz"
        } | i % 5 == 0 {
            println "Buzz"
        } | {
            println i
        }
    }
}

fn gcd: num num -> num {
    in >> a >> b
    
    out << ? b == 0 { a } | { gcd b a % b }
}

fn divmod: num num -> num num {
    in >> n >> k
    out << n / k << n % k
}

; prints "Hello, World!\n"
hello

; does fizzbuzz from 1 to 24
fizzbuzz 4 * gcd 12 18 

; prints 2 3
print divmod 11 4
