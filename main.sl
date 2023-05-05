fn gcd: num num -> num {
    a b
    ? b = 0 { a } | { gcd (b a % b) }
}

println gcd (16 36)
