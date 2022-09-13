main(argc: int, argv: [str]): int {

    sumSquare: long = 0
    squareSum: long = 0
    n: int = 100

    @ i = 1, i <= n, i = i + 1 {
        sumSquare = sumSquare + i * i
        squareSum = squareSum + i
    }

    squareSum = squareSum * squareSum

    print squareSum - sumSquare

    return 0
}