size (s: [char]): uint {
#cpp
    return s.size();
#cpp
}

countAndSay (n: int): [char] {
    ? n == 1 {
        ans: [char] = "1"
        return ans
    }

    prev: [char] = countAndSay (n - 1)
    ans: [char]

    curr: int = 0
    count: int = 1
    @ curr < size prev {
        ? curr < size prev - 1 && prev[curr] == prev[curr + 1] {
            count++
        } | {
            c: char = count + '0'
            ans = ans  + c
            ans = ans + prev[curr]
            count = 1
        }
        curr++
    }

    return ans
}

solve (n: int) {
    ans: [char] = countAndSay n
    @ i: int = 0, i < size ans, i++ {
        dump ans[i]
    }
    dumpln ""
}

main {
    solve 6
}