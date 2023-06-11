; Find the 10001st prime number

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
    p_idx: 1
    p: 1
    @ p_idx <= 10001 {
        p: p + 1
        ? is_prime p {
            p_idx: p_idx + 1
        }
    }

    print p
}

solve
