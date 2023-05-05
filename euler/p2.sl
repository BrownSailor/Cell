; Problem 2
; print the sum of all even fibonacci numbers under 4 million

fn solve: nil -> nil {
    sum: 0

    t1: 1
    t2: 2

    @ t1 < 4000000 {
        ? t1 % 2 = 0 {
            sum: sum + t1
        }

        tmp: t1
        t1: t2
        t2: t2 + tmp
    }

    println sum
}

solve
