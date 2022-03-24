use "std/std.sl"

// The prime factors of 13195 are 5, 7, 13 and 29.

// What is the largest prime factor of the number 600851475143 ?

600851475143 2 while dup2 / 1 >= do
    dup2 mod 0 = if
        // dup dump
        swap over / swap
    else
        1 +
    end
end

dump