; Find the first triangle number with over 500 divisors

fn num_factors: num -> num {
    n
    f: 0
    x: 1
    
    @ x <= n {
        ? n % x = 0 {
            f: f + 1
        }
        x: x + 1
    }
    
    f
}

fn solve: unit -> unit {
    tri: 6
    del_tri: 4

    @ (num_factors tri) <= 500 {
        tri: tri + del_tri
        del_tri: del_tri + 1
    }

    print tri
}

solve
