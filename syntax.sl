fn hello: nil -> nil {
    println "Hello, World!"
}

fn fizzbuzz: num -> nil {
    in >> N

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
    in >> a >> b

    ? b = 0 {
        out << a
    } | {
        out << gcd (b a % b)
    }
}

fn divmod: num num -> num num {
    in >> n >> k
    out << n / k << n % k
}
