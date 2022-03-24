use "std/std.sl"

// A palindromic number reads the same both ways. 
// The largest palindrome made from the product of two 2-digit numbers is 9009 = 91 × 99.

// Find the largest palindrome made from the product of two 3-digit numbers.

def ans mem end
ans 0 !64

100 while dup 1000 < do
    100 while dup 1000 < do
        dup2 *

        // reverse the current product at top of stack
        dup 0 while over 0 > do
            10 *
            over 10 % +
            swap 10 / swap
        end
        swap drop

        // product and reverse on top
        over = if
            dup ans @64 > if 
                ans swap !64
            else
                drop
            end
        else
            drop
        end

        1 +
    end drop

    1 +
end drop

ans @64 dump