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

pushb (arr: [bool], b: bool) {
#cpp
    arr.pushb(b);
#cpp
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

diff (s1: [char], s2: [char]): uint {
    ans: uint = 0
    @ i: int = 0, i < size s1, i++ {
        ? s1[i] != s2[i] {
            ans++
        }
    }

    return ans
}

wordLadder (begin: [char], end: [char], list: [[char]]): uint {
    ans: uint = 0
    queue: [[char]]
    pushb queue begin


    vis: [bool]
    @ i: int = 0, i < size list, i++ {
        pushb vis fls
    }

    @ size queue {
        add: bool = tru
        begin = popf queue

        ? begin == end {
            ans = ans - size queue
            return ans
        }

        @ i: int = 0, i < size list, i++ {
            ? diff begin list[i] == 1 && !vis[i] {
                pushb queue list[i]
                vis[i] = tru

                ans = ans + add
                add = fls
            }
        }
    }

    return 0
}

solve (begin: [char], end: [char], list: [[char]]) {
    dump wordLadder begin end list
    dumpln ""
}

main {
    begin: [char] = "hit"
    end: [char] = "cog"
    list: [[char]] = { "hot" "dot" "dog" "lot" "log" "cog" }
    solve begin end list

    list = { "hot" "dot" "dog" "lot" "log" }
    solve begin end list
}