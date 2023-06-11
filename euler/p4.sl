; Find the largest palindrome made from the product of two 3 digit numbers

fn max: num num -> num {
    a b
    ? a < b { b } | { a }
}

fn is_palindrome: num -> bool {
    n

    cur: n
    rev: 0
    @ cur != 0 {
        rev: rev * 10
        rev: rev + cur % 10
        cur: cur / 10
    }

    rev = n
}

fn solve: nil -> nil {
    ans: 0

    i: 100
    @ i < 1000 {
        j: i + 1
        @ j < 1000 {
            ? (is_palindrome i * j) {
                ans: (max ans i * j)
            }
            j: j + 1
        }
        i: i + 1
    }

    print ans
}

solve
