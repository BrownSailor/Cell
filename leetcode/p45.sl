size (arr: [int]): uint {
#cpp
    return arr.size();
#cpp
}

pushb (arr: [int], val: int) {
#cpp
    arr.pushb(val);
#cpp
}

min (a: int, b: int): int {
    ? a < b {
        return a
    }
    return b
}

jumpGame2 (arr: [int]): int {
    dp: [int]
    @ i: int = 0, i < size arr, i++ {
        pushb dp 10001
    }

    dp[size arr - 1] = 0

    @ i: int = size arr - 2, i >= 0, i-- {
        ? i + arr[i] >= size arr {
            dp[i] = 1
        } | {
            @ j: int = 1, j <= arr[i], j++ {
                dp[i] = min dp[i] (1 + dp[i + j])
            }
        }
    }

    return dp[0]
}

solve (arr: [int]) {
    dumpln jumpGame2 arr
}

main {
    nums: [int] = { 2 3 1 1 4 }
    solve nums

    nums = { 2 3 0 1 4 }
    solve nums

    nums = { 1 1 1 1 1 1 1 1 }
    solve nums
}