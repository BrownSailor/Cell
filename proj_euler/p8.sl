main(argc: int, argv: [str]): int {
    val: [char] = "7316717650"

    ans: int = 0
    @ i = 0, i < 10, i++ {
        ans = val[i]
        print ans
    }

    return 0
}