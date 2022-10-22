size (nums: [int]): uint {
#cpp
    return nums.size();
#cpp
}

size (nums: [[int]]): uint {
#cpp
    return nums.size();
#cpp
}

pushb (nums: [[int]], val: [int]) {
#cpp
    nums.pushb(val);
#cpp
}

pushb (nums: [int], val: int) {
#cpp
    nums.pushb(val);
#cpp
}

popb (nums: [int]) {
#cpp
    nums.popb();
#cpp
}

print (nums: [int]) {
    @ i: int = 0, i < size nums, i++ {
        dump nums[i]
        dump ' '
    }

    dumpln ""
}

sort (nums: [int]) {

    @ i: int = size nums - 1, i > 0, i-- {
        currMax: int = -1
        currIdx: int = -1

        @ j: int = 0, j <= i, j++ {
            ? nums[j] > currMax {
                currMax = nums[j]
                currIdx = j
            }
        }

        tmp: int = nums[i]
        nums[i] = nums[currIdx]
        nums[currIdx] = tmp
    }
}

combinationSumHelper (nums: [int], target: int, combs: [[int]], curr: [int], idx: uint) {
    ? target == 0 {
        pushb combs curr
        return
    }

    @ idx < size nums && target - nums[idx] >= 0 {
        pushb curr nums[idx]
        combinationSumHelper nums (target - nums[idx]) combs curr idx
        idx++
        popb curr 
    }
}

combinationSum (nums: [int], target: int): [[int]] {
    ans: [[int]]
    curr: [int]

    sort nums

    combinationSumHelper nums target ans curr 0

    return ans
}

solve (nums: [int], target: int) {
    ans: [[int]] = combinationSum nums target

    @ i: int = 0, i < size ans, i++ {
        print ans[i]
    }
}

// comment!
main {
    nums: [int] = { 2 3 6 7 }
    target: int = 7
    solve nums target
}