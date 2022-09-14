main(argc: int, argv: [str]): int 
{
    c: [char] = "hello!"

    @ i = 0, i < 6, i++
    {
        print c[i]
    }

    return 0
}