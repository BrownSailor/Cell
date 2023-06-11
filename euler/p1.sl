; Find the sum of all multiples of 3 or 5 below 1000

fn solve: nil -> nil {
    i: 3
    sum: 0
    @ i < 1000 {
        ? i % 3 = 0 or i % 5 = 0 {
            sum: sum + i
        }
        i: i + 1
    }

    print sum
}

solve
