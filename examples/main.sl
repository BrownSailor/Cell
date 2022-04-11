main(argc: int, argv: **char): int {

    t1: int = 4
    ? t1 < 4 {
        dump 5
    } | t1 > 4 {
        dump 6
    } | {
        dump 3
    }

    ? t1 == 21 {
        dump 14
    } | {
        dump 13
    }

    return 0
}