main: int {
    @ i = 1, i < 1000, i++ {
        @ j = 1, j < 1000, j++ {
            k: int = 1000 - j - i

            ? (i * i + j * j == k * k) {
                print i * j * k
                return 0
            }
        }
    }

    return 0
}