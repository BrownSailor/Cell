use "std/std.sl"

def N 15 end
mem a sizeof(u64) end
mem b sizeof(u64) end


0 a !64
1 b !64

0 while dup N < do
    a @64 dump
    a @64 b @64 dup
    a !64
    + b !64

    1 +
end
