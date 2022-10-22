size (s: [char]): uint {
#cpp
    return s.size();
#cpp
}

size (arr: [[char]]): uint {
#cpp
    return arr.size();
#cpp
}

pushb (s: [char], c: char) {
#cpp
    s.pushb(c);
#cpp
}

popf (s: [char]): char {
    c: char = s[0]
#cpp
    s.popf();
#cpp
    return c
}

pushb (arr: [[char]], s: [char]) {
#cpp
    arr.pushb(s);
#cpp
}

popf (arr: [[char]]): [char] {
    s: [char] = arr[0]
#cpp
    arr.popf();
#cpp
    return s
}

print (combs: [[char]]) {
    @ i: int = 0, i < size combs, i++ {
        dump combs[i]
        dump ' '
    }
    dumpln ""
}

numToLetters (digit: int): [[char]] {
    ans: [[char]]
    ? digit == 2 {
        ans = { "a" "b" "c" }
    } | digit == 3 {
        ans = { "d" "e" "f" }
    } | digit == 4 {
        ans = { "g" "h" "i" }
    } | digit == 5 {
        ans = { "j" "k" "l" }
    } | digit == 6 {
        ans = { "m" "n" "o" }
    } | digit == 7 {
        ans = { "p" "q" "r" "s" }
    } | digit == 8 {
        ans = { "t" "u" "v" }
    } | digit == 9 {
        ans = { "w" "x" "y" "z" }
    }

    return ans
}

letterCombinations (digits: [char]): [[char]] {
    combs: [[char]]
    empty: [char] = ""
    pushb combs empty

    @ size digits {
        lets: [[char]] = numToLetters (digits[0] - '0')
        ? size combs == 1 || size combs[0] != size combs[1] {
            popf digits
        }

        curr: [char] = popf combs

        @ i: int = 0, i < size lets, i++ {
            next: [char] = curr + lets[i]
            pushb combs next
        }
    }

    return combs
}

solve (digits: [char]) {
    ans: [[char]] = letterCombinations digits
    print ans
}

main {
    digits: [char] = "23"
    solve digits

    digits = ""
    solve digits

    digits = "2"
    solve digits

    digits = "234"
    solve digits
}