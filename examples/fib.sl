fib (n: int): int {
    f0: int = 0
    f1: int = 1

    ? n == 1 {
        return f0
    } | n == 2 {
        return f1
    } | {
        @ i: int = 3, i <= n, i = i + 1 {
            tmp: int = f1
            f1 = f0 + f1
            f0 = tmp
        }

        return f1
    }

    return -1
}

main (argc: int, argv: [[char]]): int {

    @ i: int = 1, i <= 10, i = i + 1 {
        dump fib i
        dump " "
    }
    dump "\n"

    return 0
}
