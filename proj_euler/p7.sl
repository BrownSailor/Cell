main(argc: int, argv: **char): int {

    curr: int = 1
    n = 10001

    @ count = 0, count < n {
        
        curr++
        b = 1
        @ j = 2, j < curr, j++ {
            ? curr % j == 0 {
                b = 0
            }
        }
        
        ? b {
            count++
        }
    }

    print curr

    return 0
}