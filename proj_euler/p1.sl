main(argc: int, argv: **char): int {

    sum: int = 0
    @ i: int = 0, i < 1000, i = i + 1 {
        sum = (i % 3 == 0 || i % 5 == 0) * i + sum
    }

    dump sum

    return 0
}