main(argc: int, argv: **char): int {

    ans: long = 1
    
    @ ans > 0 {
        ? ans % 2 == 0 && ans % 3 == 0 && ans % 4 == 0 && ans % 5 == 0 && ans % 6 == 0 && ans % 7 == 0 && ans % 8 == 0 && ans % 9 == 0 && ans % 10 == 0 && ans % 11 == 0 && ans % 12 == 0 && ans % 13 == 0 && ans % 14 == 0 && ans % 15 == 0 && ans % 16 == 0 && ans % 17 == 0 && ans % 18 == 0 && ans % 19 == 0 && ans % 20 == 0 {
            dump ans
            return 0
        }
        ans = ans + 1
    }

    return 0
}