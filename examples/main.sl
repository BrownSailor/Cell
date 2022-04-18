main(argc: int, argv: **char): int {
    x: int = 0

    @ i: int = 0, i < 5, i = i + 1 {
        dump x
    }

    return x
}