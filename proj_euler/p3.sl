main(argc: int, argv: **char): int {

    num: long = 600851475143
    ans: long = 1

    @ num > ans {
        ? num % ans == 0 {
            num = num / ans
        } | {
            ans = ans + 1
        }
    }

    dump ans

    return 0
}