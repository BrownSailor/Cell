; Find the product abc of the pythagorean triplet where a + b + c = 1000

fn solve: unit -> unit {
    ans: 0

    a: 1
    @ a <= 1000 and ans = 0 {
        b: a + 1
        @ b <= 1000 and ans = 0 {
            c: 1000 - a - b
            ? a * a + b * b = c * c {
                ans: a * b * c
            }

            b: b + 1
        }

        a: a + 1
    }

    print ans
}

solve
