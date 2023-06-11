; Find the sum of the even Fibonacci terms that are less than 4 million

fn solve: unit -> unit {
    sum: 0
    t1: 1
    t2: 2

    @ t1 < 4000000 {
        ? t1 % 2 = 0 {
            sum: sum + t1
        }

        tmp: t2
        t2: t1 + t2
        t1: tmp
    }

    print sum
}

solve
