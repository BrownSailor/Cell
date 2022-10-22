fact (n: int): int {
    ? n == 0 {
        return 1
    }

    return n * fact (n - 1)
}

main (argc: int, argv: [[char]]): int {

    dumpln fact 6
    return 0
}
