valid (grid: [[int]], row: int, col: int): bool {
    return (col >= 0 && col < size (grid[0]) && row >= 0 && row < size (grid))
}

bfs (grid: [[int]], vis: [[bool]], row: int, col: int, target: int): bool {
    queue: [[int]]
    start: [int] = { row col }
    pushb (queue start)

    i: int = 0

    @ size (queue) {

        row = queue[0][0]
        col = queue[0][1]
        popf (queue)

        ? grid[row col] == target {
            return 1
        }

        vis[row col] = 1

        ? valid (grid row + 1 col) && !vis[row + 1 col] {
            next: [int] = { row + 1 col }
            pushb (queue next)
        }
        ? valid (grid row - 1 col) && !vis[row - 1 col] {
            next: [int] = { row - 1 col }
            pushb (queue next)
        }
        ? valid (grid row col + 1) && !vis[row col + 1] {
            next: [int] = { row col + 1 }
            pushb (queue next)
        }
        ? valid (grid row col - 1) && !vis[row col - 1] {
            next: [int] = { row col - 1 }
            pushb (queue next)
        }
    }

    return 0
}

main {

    grid: [[int]] = { { 1 2 3 4 } { 5 6 7 8 } { 9 10 11 12 } }
    vis: [[bool]] = { { 0 0 0 0 } { 0 0 0 0 } { 0 0 0 0 } }

    ? bfs (grid vis 0 0 15) {
        dumpln "Found target :)"
    } | {
        dumpln "Did not find target :("
    }
}
