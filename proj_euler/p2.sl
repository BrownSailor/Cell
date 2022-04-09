main {

    t1:  int = 1
    t2:  int = 2
    
    @ 0, t1 < 100, 0 {
        dump t1
        tmp: int = t2
        t2 = t1 + t2
        t1 = tmp
    }
}