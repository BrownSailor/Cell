use "std/std.sl"

// Multiples of 3 or 5
// If we list all the natural numbers below 10 that are multiples of 3 or 5, we get 3, 5, 6 and 9. The sum of these multiples is 23.
// Find the sum of all the multiples of 3 or 5 below 1000.

0 3 while dup 1000 < do
    if
    dup  3 mod 0 =
    over 5 mod 0 =
    or do
        swap over + swap
    end

    1 +
end

drop
dump

