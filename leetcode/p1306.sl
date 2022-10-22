size (arr: [int]): uint {
#cpp
    return arr.size();
#cpp
}

size (arr: [bool]): uint {
#cpp
    return arr.size();
#cpp
}

pushb (arr: [int], val: int) {
#cpp
    arr.pushb(val);
#cpp
}

popf (arr: [int]): int {
    val: int = arr[0]
#cpp
    arr.popf();
#cpp
    return val
}

jumpGame3 (arr: [int], start: int): bool {
    queue: [int]
    vis: [int]

    @ i: int = 0, i < size arr, i++ {
        pushb vis fls
    }

    pushb queue start

    @ size queue {
        start = popf queue
        vis[start] = tru

        ? arr[start] == 0 {
            return tru
        }

        ? start + arr[start] < size arr && !vis[start + arr[start]] {
            pushb queue (start + arr[start])
        }

        ? start - arr[start] > 0 && !vis[start - arr[start]] {
            pushb queue (start - arr[start])
        }
    }

    return fls
}

solve (arr: [int], start: int) {
    ? jumpGame3 arr start {
        dumpln "tru"
    } | {
        dumpln "fls"
    }
}

main {
    nums: [int] = { 4 2 3 0 3 1 2 }
    start: int = 5
    solve nums start

    start = 0
    solve nums start

    nums = { 3 0 2 1 2 }
    start = 2
    solve nums start
}