main(argc: int, argv: **char): int {

    t1: int = 2
    ? t1 < 4 {
        dump 5
    }

    ? t1 > 1 {
        dump 2
    }

    ? t1 == 2 {
        dump 14
    }

    return 0
}