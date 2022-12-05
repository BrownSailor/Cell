size (arr: [int]): uint {
#cpp
    return arr.size();
#cpp
}

solve (arr: [int]) {
    newArr: [int] = arr

    @ i: int = 0, i < size newArr, i++ {
        ? newArr[i] == 0 {
            newArr[i] = 1
        } | newArr[i] == 1 {
            newArr[i] = -1
        }
    }
    
    left:  int = 0
    right: int = 0
    score: int = newArr[0]

    currScore: int = 0
    currLeft: int = 0

    @ i: int = 0, i < size newArr, i++ {
        currScore = currScore + newArr[i]

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
    dumpln " ]"
}

main {
    arr: [int] = { 1 0 0 1 0 0 1 1 1 1 1 1 0 }
    solve arr
}