fact (n: int): int {
    ? n == 0 {
        return 1
    }

    return n * fact (n - 1)
}

main {

    dumpln fact 6
}
