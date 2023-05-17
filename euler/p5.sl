; Problem 5
; print the smallest number divisible by every number between 1 and 20 (inclusive)

fn gcd: num num -> num {
    a b
    ? b = 0 { a } | { gcd (b a % b) }
}

fn solve: nil -> nil {
    N: 20

    ans: 2

    i: 3
    @ i <= N {
        ans: ans * i / gcd (ans i)
        i: i + 1
    }
    
    println ans
}

solve
