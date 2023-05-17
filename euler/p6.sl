; Problem 6
; print the difference between the sum of the squares of the first one hundred natural numbers and the square of the sum

fn solve: nil -> nil {
    N: 100

    sq_sum: (N * N + N) / 2
    sq_sum: sq_sum * sq_sum

    sum_sq: 0
    i: 1
    @ i <= N {
        sum_sq: sum_sq + i * i
        i: i + 1
    }

    ans: sq_sum - sum_sq

    println ans
}

solve
