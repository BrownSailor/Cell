main(argc: int, argv: [str]): int {
    sum: int = 0

    @ x: int = 2, x < 10, x++ {
        b = 1
        @ i = 2, i < x && b, i++ {
            ? x % i == 0 {
                b = 0
            }
        }

        sum = sum + b * x
    }   

    print sum

    return 0
}