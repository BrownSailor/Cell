main(argc: int, argv: **char): int {
    x: int = 0

    @ i: int = 0, i < 5, i++ {
        dump i
    }

    return 0
}