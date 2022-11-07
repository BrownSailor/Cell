main {
    @ i: int = 0, i < 10, i++ {
        dump i
        dump ' '
    }
    dumpln ""

    count: int = 0
    @ count < 15, count++ {
        dump count
        dump ' '
    }
    dumpln ""

    count = 0
    @ count < 15 {
        dump ++count
        dump ' '
    }
    dumpln ""
}