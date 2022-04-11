main(argc: int, argv: **char): int {

    t1:  int = 1
    t2:  int = 2
    sum: int = 0

    @ t1 < 4000000 {
        ? t1 % 2 == 0 {
            sum = sum + t1
        }

        tmp: int = t2
        t2 = t1 + t2
        t1 = tmp
    }

    dump sum

    return 0
}