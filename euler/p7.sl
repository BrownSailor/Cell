; Problem 7
; print the 10001st prime number

fn solve: nil -> nil {
    N: 10001

    primes: 10001 * num
    primes[1]: 2

    i: 2
    @ i <= N {
        cur_prime: primes[i - 1] + 1
        j: i - 1
        @ j > 0 {
            ? cur_prime % primes[j] = 0 {
                cur_prime: cur_prime + 1
                j: i
            }
            j: j - 1
        }
        primes[i]: cur_prime
        i: i + 1
    }

    println primes[N]
}

solve
