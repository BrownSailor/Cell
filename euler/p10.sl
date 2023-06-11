; Find the sum of all primes below two million

fn is_prime: num -> bool {
    n
    b: tru

    i: 2
    @ i < n and b {
        ? n % i = 0 {
            b: fls
        }
        i: i + 1
    }

    b
}

fn solve: unit -> unit {
    p: 2
    sum: 0

    @ p < 2000000 {
        ? is_prime p {
            sum: sum + p
        }
        p: p + 1
    }

    print sum
}

solve
