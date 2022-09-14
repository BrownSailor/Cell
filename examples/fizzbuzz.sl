main(argc: int, argv: [str]): int 
{
    @ i = 1, i <= 20, i++
    {
        ? i % 3 == 0 {
            print "Fizz"
        } | i % 5 == 0 {
            print "Buzz"
        } | i % 3 == 0 && i % 5 == 0 {
            print "FizzBuzz"
        } | {
            print i
        }
    }
    
    return 0
}