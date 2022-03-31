use "std/std.sl"

// random numbers between 0 and 127
def M 128 end

def N 10 end
mem arr sizeof(u64) N * end
mem i sizeof(u64) end

def initArr
    0 while dup N < do
        dup sizeof(u64) * arr + rand M mod !64
        1 +
    end drop
end

def dumpArr
    0 while dup N < do 
        dup sizeof(u64) * arr + @64 dump 

        1 +
    end drop
end

// j and j + 1 at top of stack
// leave j at top of stack
mem tmp sizeof(u64) end // store arr[j + 1]
tmp 0 !64

def swapAdj
    // tmp = arr[j + 1]
    tmp over sizeof(u64) * arr + @64 !64

    // arr[j + 1] = arr[j]
    over sizeof(u64) * arr + @64
    over sizeof(u64) * arr + swap !64

    // arr[j] = tmp
    over tmp @64 swap sizeof(u64) * arr + swap !64
    drop
end

def sortArr
    0 while dup N 1 - < do
        0 while over N 1 - swap - over > do
            dup 1 +

            dup2 
            sizeof(u64) * arr + @64 swap
            sizeof(u64) * arr + @64 swap

            dump dump drop
            // if > do
            //     swapAdj
            // end

            1 +
        end drop

        1 +
    end drop
end

initArr

"Unsorted array:\n" stdout write drop
dumpArr

"\nSorted array:\n" stdout write drop
sortArr
// dumpArr
