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

twoSum (arr: [int], target: int): [int] {
    ans: [int]

    @ i: int = 0, i < size arr, i++ {
        @ j: int = i + 1, j < size arr, j++ {
            ? arr[i] + arr[j] == target {
                pushb ans i
                pushb ans j

                return ans
            }
        }
    }

    return ans
}

solve (nums: [int], target: int) {
    ans: [int] = twoSum nums target

    dump ans[0]
    dump ' '
    dump ans[1]
    dumpln ""
}

main {
    nums: [int] = { 2 7 11 15 }
    target: int = 9
    solve nums target

    nums = { 3 2 4 }
    target = 6
    solve nums target

    nums = { 3 3 }
    target = 6
    solve nums target
}