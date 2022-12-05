size (arr: [int]): uint {
#cpp
    return arr.size();
#cpp
}

solve (arr: [int]) {
    left:  int = 0
    right: int = 0
    score: int = arr[0]

    currScore: int = 0
    currLeft: int = 0

    @ i: int = 0, i < size arr, i++ {
        currScore = currScore + arr[i]

        ? currScore > score {
            score = currScore
            left = currLeft
            right = i
        }

        ? currScore < 0 {
            currScore = 0
            currLeft = i + 1
        }
    }

    dump "[ "
    dump left
    dump " "
    dump right
    dump " ]: "
    dumpln score
}

main {
    arr: [int] = { -2,1,-3,4,-1,2,1,-5,4 }
    solve arr
}