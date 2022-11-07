main {
    x: int = 32
    y: int = 14
    z: int = x + y

    c: char = 'c'
    s: [char] = "Hello!"

    dumpln z
    dumpln c
    dumpln s

    dumpln "Hi!"
    dumpln 'a'

    dumpln x + y + z

    return 0
}
