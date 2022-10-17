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

swap (arr: [int], a: uint, b: uint) {
    tmp: int = arr[a]
    arr[a] = arr[b]
    arr[b] = tmp
}

swap (arr: [[int]], i: uint, j: uint, x: uint, y: uint) {
    tmp: int = arr[i j]
    arr[i j] = arr[x y]
    arr[x y] = tmp
}

rotateImage (arr: [[int]]) {
    @ i: int = 0, i < size (arr) / 2, i++ {
        @ j: int = 0, j < size arr, j++ {
            swap arr i j size (arr) - i - 1 j
        }
    }

    @ i: int = 0, i < size arr, i++ {
        @ j: int = i, j < size arr, j++ {
            swap arr i j j i
        }
    }
    
    @ i: int = 0, i < size arr, i++ {
        @ j: int = 0, j < size arr, j++ {
            dump arr[i j]
            dump ' '
        }
        dumpln ""
    }
}

solve (arr: [[int]]) {
    rotateImage arr
}

main {
    arr: [[int]] = { { 1 2 3 } { 4 5 6 } { 7 8 9 } }
    solve arr

    arr = { { 5 1 9 11 } { 2 4 8 10 } { 13 3 6 7 } { 15 14 12 16 } }
    solve arr
}