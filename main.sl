fn hello: nil -> nil {
    print "Hello, World!"
}

fn gcd: num num -> num {
    a b
    ? b = 0 { a } | { gcd b a % b }
}

fn bye: nil -> nil {
    var: "Goodbye, World!"
    print var
}

hello
print (gcd 30 12)
bye
