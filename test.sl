fn mul: num num -> num {
    in >> n >> k
    out << n * k
}

fn divmod: num num -> num num {
    in >> n >> k
    out << n / k << n % k
}

println (mul 3 4)
