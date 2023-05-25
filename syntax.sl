fn hello: nil -> nil {
    println "Hello, World!"
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

    ? b = 0 {
        a
    } | {
        gcd (b a % b)
    }
}

fn divmod: num num -> num num {
    n k

    n / k
    n % k
}

fn prime: num -> bool {
    N

    not_primes: N * bool
    not_primes[1]: tru

    i: 2
    @ i <= N {

        j: i * 2
        @ j <= N {
            not_primes[j]: tru

            j: j + i
        }

        i: i + 1
    }

    not not_primes[N]
}
