size (arr: [int]): int {
#cpp
    return arr.size();
#cpp
}

insertionSort (arr: [int]) {

    @ i: int = size arr - 1, i > 0, i-- {
        currIdx: int = -1
        currMax: int = -1

        @ j: int = 0, j <= i, j++ {
            ? arr[j] > currMax {
                currMax = arr[j]
                currIdx = j
            }
        }

        tmp: int = arr[i]
        arr[i] = currMax
        arr[currIdx] = tmp
    }
}

binarySearch (arr: [int], start: int, end: int, target: int): int {

    ? start < end {
        mid: int = (start + end) / 2

        ? arr[mid] == target {
            return mid
        } | arr[mid] > target {
            return binarySearch arr start mid target
        } | {
            return binarySearch arr (mid + 1) end target
        }
    }

    return -1
}

main (argc: int, argv: [[char]]): int {

    arr: [int] = { 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 }
    insertionSort arr

    dumpln arr[binarySearch arr 0 20 binarySearch arr 0 20 15]

    return 0
}
