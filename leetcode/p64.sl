size (arr: [[int]]): uint {
#cpp
    return arr.size();
#cpp
}

size (arr: [int]): uint {
#cpp
    return arr.size();
#cpp
}

pushb (arr: [[int]], val: [int]) {
#cpp
    arr.pushb(val);
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
    } | {
        return b
    }
}

minPathSum (arr: [[int]]): int {
    dp: [[int]]
    m: uint = size arr
    n: uint = size arr[0]

    @ i: int = 0, i < size arr, i++ {
        a: [int]
        @ j: int = 0, j < size arr[i], j++ {
            pushb a 0
        }
        pushb dp a
    }

    dp[m - 1 n - 1] = arr[m - 1 n - 1]

    @ i: int = m - 2, i >= 0, i-- {
        dp[i n - 1] = dp[i + 1 n - 1] + arr[i n - 1]
    }

    @ i: int = n - 2, i >= 0, i-- {
        dp[m - 1 i] = dp[m - 1 i + 1] + arr[m - 1 i]
    }

    @ i: int = m - 2, i >= 0, i-- {
        @ j: int = n - 2, j >= 0, j-- {
            dp[i j] = arr[i j] + min dp[i + 1 j] dp[i j + 1]
        }
    }

    return dp[0 0]
}

main {

    arr: [[int]] = { { 1 3 1 } { 1 5 1 } { 4 2 1 } }
    dumpln minPathSum arr

    arr1: [[int]] = { { 1 2 3 } { 4 5 6 } }
    dumpln minPathSum arr1
}