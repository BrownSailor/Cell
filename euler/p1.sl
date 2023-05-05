; Problem 1
; print the sum of all numbers under 1000 divisible by 3 or 5

fn solve: nil -> nil {
    sum: 0
    i: 0

    @ i < 1000 {
        ? i % 3 = 0 or i % 5 = 0 {
            sum: sum + i
        }

        i: i + 1
    }

    println sum
}

solve
