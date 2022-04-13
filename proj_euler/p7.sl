main(argc: int, argv: **char): int {

    curr: int = 1
    n: int = 10001

    @ count = 0, count < n {
        
        curr = curr + 1
        b: bool = 1
        @ j = 2, j < curr, j = j + 1 {
            ? curr % j == 0 {
                b = 0
            }
        }
        
        ? b {
            count = count + 1
        }
    }

    dump curr

    return 0
}