size (arr: [int]): uint {
#cpp
    return arr.size();
#cpp
}

findPivot (arr: [int], left: int, right: int): int {
    ? left <= right {
        mid: int = (right + left) / 2
        ? arr[mid] > arr[mid + 1] {
            return mid
        } | arr[(size arr) - 1] < arr[mid] {
            return findPivot arr mid + 1 right
        } | {
            return findPivot arr left mid - 1
        }
    }

    return -1
}

binSearch (arr: [int], left: int, right: int, target: int): int {
    ? left <= right {
        mid: int = (right + left) / 2
        ? arr[mid] == target {
            return mid
        } | arr[mid] > target {
            return binSearch arr left mid - 1 target
        } | {
            return binSearch arr mid + 1 right target
        }
    }

    return -1
}

solve (arr: [int], target: int) {
    pivot: int = findPivot arr 0 size arr target
    ans: int = binSearch arr 0 pivot target

    ? ans == -1 {
        ans = binSearch arr pivot + 1 size arr target
    }

    dumpln ans
}

main {
    arr: [int] = { 4 5 6 7 0 1 2 }
    solve arr 0
    solve arr 3
}