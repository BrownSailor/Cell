func: char
{

    return 'a'
}

fact(n: int): int
{
    ? n == 0
    {
        return 1
    }
    
    return n * fact(n - 1)
}

main: int {

    var1: int = 3
    var2: [char] = "hello"

    @ i: int = 0, i < 15, i++ {
        ? i < 3 {
            print i
        } | i > 5 {
            print i
        } | {
            print i
        }
    }

    return 0
}