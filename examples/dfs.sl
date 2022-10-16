valid (grid: [[int]], row: int, col: int): bool {
    return (col >= 0 && col < size (grid[0]) && row >= 0 && row < size (grid))
}

dfs (grid: [[int]], vis: [[int]], row: int, col: int, target: int): bool {

    vis[row col] = 1

    ? grid[row col] == target {
        return 1
    }

    ? valid (grid row + 1 col) && vis[row + 1 col] == 0 {
        return dfs (grid vis row + 1 col target)
    } | valid (grid row - 1 col) && vis[row - 1 col] == 0 {
        return dfs (grid vis row - 1 col target)
    } | valid (grid row col + 1) && vis[row col + 1] == 0 {
        return dfs (grid vis row col + 1 target)
    } | valid (grid row col - 1) && vis[row col - 1] == 0 {
        return dfs (grid vis row col - 1 target)
    }

    return 0
}

main (argc: int, argv: [[char]]): int {

    grid: [[int]] = { { 1 2 3 4 } { 5 6 7 8 } { 9 10 11 12 } }
    vis: [[int]] = { { 0 0 0 0 } { 0 0 0 0 } { 0 0 0 0 } }

    ? dfs (grid vis 0 0 15) {
        dumpln "Found target :)"
    } | {
        dumpln "Did not find target :("
    }

    return 0
}
