main (argc: int, argv: [[char]]): int
{

    ? 1 > 1 {
        dumpln "not cool"
    }

    ? 2 < 1 {
        dumpln "not cool"
    } | {
        dumpln "cool"
    }

    ? 2 < 1 {
        dumpln "not cool"
    } | 1 < 2 {
        dumpln "cool"
    } | {
        dumpln "not cool"
    }

    ? 2 < 1 {
        dumpln "not cool"
    } | 3 < 2 {
        dumpln "not cool"
    } | {
        ? 3 > 2 {
            dumpln "cool"
        }
    }

    return 0
}
