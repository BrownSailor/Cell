use "std/std.sl"

// The sum of the squares of the first ten natural numbers is,
// 1 + 4 + 9 + ... + 100 = 385

// The square of the sum of the first ten natural numbers is,
// (1 + 2 + 3 + ... + 10)(1 + 2 + 3 + ... + 10) = 3025

// Hence the difference between the sum of the squares of the first ten natural numbers and the square of the sum is
// 3025 - 385 = 2640

// Find the difference between the sum of the squares of the first one hundred natural numbers and the square of the sum.

def N 100 end

// sum of squares
0 1 while dup N <= do
    swap over dup * + swap

    1 +
end drop

// square of sums
0 1 while dup N <= do
    swap over + swap

    1 +
end drop
dup *

swap - dump