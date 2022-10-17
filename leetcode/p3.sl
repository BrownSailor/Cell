size (arr: [int]): uint {
#cpp
    return arr.size();
#cpp
}

size (s: [char]): uint {
#cpp
    return s.size();
#cpp
}

pushb (arr: [int], val: int) {
#cpp
    arr.pushb(val);
#cpp
}

lengthOfLongestSubstring (s: [char]): int {
    map: [int]
    @ i: int = 0, i < 26, i++ {
        pushb map (-1)
    }

    max: int = 1
    curr: int = 0
    left: int = 0
    right: int = 1

    map[s[left] - 'a'] = left
    @ right < size s {
        ? map[s[right] - 'a'] == -1 {
            map[s[right] - 'a'] = right
            right++
            curr++
        } | {
            curr--
            left++
            map[s[left] - 'a'] = -1
        }

        ? max < curr {
            max = curr
        }
    }

    return max
}

solve (s: [char]) {
    dumpln s
    dumpln lengthOfLongestSubstring s
}

main {
    s: [char] = "abcabcbb"
    solve s

    s = "bbbbb"
    solve s

    s = "pwwkew"
    solve s
}