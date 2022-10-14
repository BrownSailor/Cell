main (argc: int, argv: [[char]]): int {

    val: [char] = "hello"
    dumpln val

    val2: [char] = val

    val = "abacus"
    dumpln val

    dumpln val2

    val2 = val

    dumpln val2

    return 0
}
