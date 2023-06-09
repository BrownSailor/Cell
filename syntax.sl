fn hello: nil -> nil {
    print "Hello, World!"
}

fn gcd: num num -> num {
    a b
    ? b = 0 { a } | { gcd b a % b }
}

hello
print (gcd 30 12)
