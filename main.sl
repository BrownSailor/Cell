fn hello: nil -> nil {
    print "Hello, World!"
}

fn bye: str -> nil {
    var
    print var
}

fn gcd: num num -> num {
    a b
    ? b = 0 { a } | { gcd b a % b }
}

fn fizzbuzz: num -> nil {
    N

    i: 1
    @ i <= N {
        ? i % 3 = 0 and i % 5 = 0 {
            print "FizzBuzz"
        } | i % 3 = 0 {
            print "Fizz"
        } | i % 5 = 0 {
            print "Buzz"
        } | {
            print i
        }

        i: i + 1
    }
}

hello
print "--------------------"
bye "Goodbye, World!"
print "--------------------"
print (gcd 30 12)
print "--------------------"
fizzbuzz 20
