; Problem 1
; print the sum of all numbers under 1000 divisible by 3 or 5

; fn solve: nil -> nil {
;     sum: 0
;     i: 0
; 
;     @ i < 1000 {
;         ? i % 3 = 0 or i % 5 = 0 {
;             sum: sum + i
;         }
; 
;         i: i + 1
;     }
; 
;     println sum
; }
; 
; solve

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
