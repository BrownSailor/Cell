; Find the smallest positive number evenly divisible by all numbers between 1 and 20

fn gcd: num num -> num {
    a b
    ? b = 0 { a } | { (gcd b a % b) }
}

fn solve: nil -> nil {
    ans: 1

    i: 2
    @ i <= 20 {
        ans: ans * i / (gcd ans i)
        i: i + 1
    }

    print ans
}

solve
