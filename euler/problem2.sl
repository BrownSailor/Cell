use "std/std.sl"

// Each new term in the Fibonacci sequence is generated by adding the previous two terms. 
// By starting with 1 and 2, the first 10 terms will be:

// 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...

// By considering the terms in the Fibonacci sequence whose values do not exceed four million, 
// find the sum of the even-valued terms.

mem sum 8 end
sum 0 !64

1 2 while over 4000000 < do
    over 
    dup 2 mod 0 = if 
        sum swap sum @64 + !64
    else
        drop
    end
    swap over +
end

sum @64 dump