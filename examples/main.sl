main(argc: int, argv: [str]): int 
{
    println argc
    @ i = 0, i < argc, i++
    {
        print argv[i]
    }

    return 0
}