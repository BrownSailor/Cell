main(argc: int, argv: [str]): int {

    num: long = 600851475143
    ans: long = 2

    @ num > ans {
        ? num % ans == 0 {
            num = num / ans
        } | {
            ans = ans + 1
        }
    }

    print ans

    return 0
}