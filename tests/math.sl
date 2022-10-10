main (argc: int, argv: [[char]]): int
{
    val1: int = 3 + 3
    val2: int = val1 * val1

    dumpln val2 / val1 + val1
    dumpln val2 % val1 + (val2 * val1 / val2) - val1

    dumpln val1 && val2 && val1 && val2 && val1 && val2 && val1 && val2 && val1 && val2 && 0

    return 0
}
