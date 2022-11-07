main {

    // variable declaration and assignment failures
    vda1: bool = tru
    // vda2: bool = 'c'
    // vda3: bool = 0
    // vda4: bool = "hi"
    // vda5: bool = { 1 2 3 }
    // vda6: bool = { (1 == 0) (2 == 0) }

    // variable assignment failures
    va1: bool  = tru
    // va1 = 3
    // va1 = "hi"
    // va1 = bool
    va1 = 2 == 3

    dumpln vda1
    dumpln va1
}