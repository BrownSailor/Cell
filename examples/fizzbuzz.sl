main(argc: int, argv: **char): int 
{
    @ i: int = 1, i < 20, i++
    {
        ? i % 3 == 0 {
            print "Fizz"
        } | i % 5 == 0 {
            print "Buzz"
        } | i % 3 == 0 & i % 5 == 0 {
            print "FizzBuzz"
        } | {
            print i
        }
    }
    
    return 0
}