; Problem 4
; print the largest palindromic number that is a product of two 3 digit numbers

fn is_palindrome: num -> bool {
    N

    rev: 0
    cur: N
    @ cur > 0 {
        rev: rev * 10
        rev: rev + cur % 10
        cur: cur / 10
    }

    rev = N
}

fn solve: nil -> nil {
    ans: 0

    i: 999
    @ i > 99 {
        j: 999
        @ j > 99 {
            k: i * j
            ? is_palindrome k {
                ans: ? k > ans { k } | { ans }
            }
            j: j - 1
        }
        i: i - 1
    }

    println ans
}

solve
