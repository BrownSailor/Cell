main(argc: int, argv: [str]): int {

    sum: int = 0
    @ i: int = 0, i < 1000, i = i + 1 {
        sum = (i % 3 == 0 || i % 5 == 0) * i + sum
    }

    print sum

    return 0
}