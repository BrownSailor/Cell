var0: 3
var1: tru
var2: 3 >= 4
var3: not (not var1) and (not var2)

println var0
println var1
println var2
println var3

i: 3
@ i < 10 {
    println i
    i: i + 1
}
