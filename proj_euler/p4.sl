main(argc: int, argv: **char): int {

    max: int = 0
    @ i: int = 100, i < 1000, i = i + 1 {
        @ j: int = 100, j < 1000, j = j + 1 {
            prod: int = i * j
            tmp: int = prod
            rev: int = 0
            @ tmp {
                rev = (rev * 10) + (tmp % 10)
                tmp = tmp / 10
            }

            ? rev == prod {
                ? prod > max {
                    max = prod
                }
            }
        }
    }

    dump max

    return 0
}