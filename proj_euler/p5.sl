main(argc: int, argv: **char): int {

    ans: long = 1
    
    @ ans > 0, ans++ {
        b: bool = 1

        @ i: int = 2, i <= 20, i++ {
            ? ans % i != 0 {
                b = 0
            }
        }

        ? b {
            print ans
            return 0
        }
    }

    return 0
}