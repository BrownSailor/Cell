main {

    ? 1 > 1 {
        dumpln "not cool"
    }

    ? 2 < 1 {
        dumpln "not cool"
    } | {
        dumpln "2 cool"
    }

    ? 2 < 1 {
        dumpln "not cool"
    } | 1 < 2 {
        dumpln "3 cool"
    } | {
        dumpln "not cool"
    }

    ? 2 < 1 {
        dumpln "not cool"
    } | 3 < 2 {
        dumpln "not cool"
    } | {
        ? 3 > 2 {
            dumpln "5 cool"
        }
    }
}
