use "std/std.sl"

// A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,

// a^2 + b^2 = c^2
// For example, 3^2 + 4^2 = 9 + 16 = 25 = 5^2.

// There exists exactly one Pythagorean triplet for which a + b + c = 1000.
// Find the product abc.

mem a sizeof(u32) end
mem b sizeof(u32) end
mem c sizeof(u32) end

1 while dup 1000 < do
    dup a swap !32
    1 while dup 1000 < do
        dup b swap !32
        1000 a @32 - b @32 - c swap !32

        if a @32 dup * b @32 dup * + c @32 dup * = do
            // a @32 dump
            // b @32 dump
            // c @32 dump
            a @32 b @32 * c @32 * dump

            0 exit
        end
        
        1 +
    end drop

    1 +
end drop
