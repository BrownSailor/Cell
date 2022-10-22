size (arr: [int]): uint {
#cpp
    return arr.size();
#cpp
}

jumpGame (arr: [int]): bool {
    dp: int = size arr

    @ i: int = size arr - 1, i >= 0, i-- {
        ? i + arr[i] >= dp {
            dp = i
        }
    }

    return dp == 0
}

solve (arr: [int]) {
    ? jumpGame arr {
        dumpln "tru"
    } | {
        dumpln "fls"
    }
}

// main program
main {
    nums: [int] = { 2 3 1 1 4 }
    solve nums

    nums = { 3 2 1 0 4 }
    solve nums
}