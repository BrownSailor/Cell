use "std/std.sl"

// The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.

// Find the sum of all the primes below two million.

mem sum    sizeof(u64) end
mem count  sizeof(u64) end
mem primes sizeof(u64) 10000000 * end    // assume < 10M primes below 2M

sum 2 !64

def isPrime
    0
    while
        if dup2 sizeof(u64) * primes + @64 dup * >= do
            dup2 sizeof(u64) * primes + @64 mod 0 !=
        else
            fls
        end
    do 
        1 + 
    end
    sizeof(u64) * primes + @64 dup * <
end

def addPrime
  primes count @64 sizeof(u64) * + swap !64
  count inc64
end

2 addPrime

3 while dup 2000000 < do
    if dup isPrime do
        dup addPrime 
        sum sum @64 primes count @64 1 - sizeof(u64) * + @64 + !64
    end

    2 +
end drop

sum @64 dump