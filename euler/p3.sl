; Find the largest prime factor of 600851475143

fn is_prime: num -> bool {
    N

    i: 2
    b: tru
    @ i < N and b {
        ? N % i = 0 {
            b: fls
        }

        i: i + 1
    }

    b
}

fn solve: unit -> unit {
    n: 600851475143
    p: 2
    
    @ n / p > 0 {
        ? n % p = 0 {
            n: n / p
        } | {
            p: p + 1
            ? not (is_prime p) {
                p: p + 1
            }
        }
    }
    
    print p
}

solve
