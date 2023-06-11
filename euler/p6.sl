; Find the difference between the sum of the squares of the first one hundred natural numbers and the square of the sum.

fn solve: unit -> unit {
    sq_sum: 50 * 50 * 101 * 101
    sum_sq: 0

    i: 1
    @ i <= 100 {
        sum_sq: sum_sq + i * i
        i: i + 1
    }

    print sq_sum - sum_sq
}

solve
