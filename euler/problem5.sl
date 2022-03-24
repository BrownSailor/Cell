use "std/std.sl"

// 2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.

// What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?

def N 20 end
def pfz mem end
def arr pfz 8 N * + end
def ans arr 8 N * + end

def inc64 
    dup @64 1 + !64
end

2 while dup N <= do

    // fill arr with 0
    0 while dup N < do
        arr over 8 * + 0 !64
        1 +
    end drop

    // factorization
    dup 2 while over 1 != do
        dup2 mod 0 = if
            dup 8 * arr + inc64
            swap over / swap
        else
            1 +
        end
    end drop2

    // print table
    // dup dump
    // 0 while dup N < do
    //     " " stdout write drop
    //     arr over 8 * + @64 dump
    //     1 +
    // end drop

    0 while dup N < do
        dup  8 * pfz + @64
        over 8 * arr + @64
        < if
            dup  8 * pfz +
            over 8 * arr + @64
            !64
        end
        
        1 +
    end drop

    1 +
end drop

// prime factorization of the answer
// 0 while dup N <= do
//     pfz over 8 * + @64
//     "  " stdout write drop
//     dump
//     dup dump
//     "------\n" stdout write drop
//     1 +
// end drop

// store product of factors in memory
ans 1 !64
1 while dup N <= do
    pfz over 8 * + @64 while dup 0 > do
        over ans @64 * ans swap !64

        1 -
    end drop

    1 +
end drop

ans @64 dump