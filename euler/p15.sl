; Find the number of lattice paths in a 20 x 20 grid

fn choose: num num -> num {
    n k

    ? k = 0 {
        1
    } | n = 0 {
        0
    } | {
        (choose n - 1 k - 1) + (choose n - 1 k)
    }
}

fn solve: unit -> unit {
    print (choose 4 2)
}

solve
