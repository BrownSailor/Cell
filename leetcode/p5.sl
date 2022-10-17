size (s: [char]): uint {
#cpp
    return s.size();
#cpp
}

size (arr: [bool]): uint {
#cpp
    return arr.size();
#cpp
}

size (arr: [[bool]]): uint {
#cpp
    return arr.size();
#cpp
}

pushb (arr: [bool], val: int) {
#cpp
    arr.pushb(val);
#cpp
}

pushb (arr: [[bool]], val: [bool]) {
#cpp
    arr.pushb(val);
#cpp
}

longestPalindrome (s: [char]) {
    n: int = size s
    dp: [[bool]]
    len: int = 0
    left: int = 0
    right: int = 0

    @ i: int = 0, i < n, i++ {
        v: [bool]
        @ j: int = 0, j < n, j++ {
            pushb v fls
        }
        pushb dp v
    }

    @ i: int = 0, i < n, i++ {
        dp[i i] = tru
    }

    @ end: int = 0, end < n, end++ {
        @ start: int = end - 1, start >= 0, start-- {
            dp[start end] = (s[start] == s[end]) && (end - start == 1 || dp[start + 1 end - 1])

            ? dp[start end] && len < end - start {
                len = end - start
                left = start
                right = end
            }
        }
    }

    @ i: int = left, i <= right, i++ {
        dump s[i]
    }
    dumpln ""
}

solve (s: [char]) {
    longestPalindrome s
}

main {
    s: [char] = "babad"
    solve s

    s = "cbbd"
    solve s
}