fn sq: num -> num {
    in >> n
    out << n * n
}

fn mul: num num -> num {
    in >> n >> k
    out << n * k
}

fn divmod: num num -> num num {
    in >> n >> k
    out << n / k << n % k
}

(println (mul (divmod 3 4)))
(println (sq 3))
