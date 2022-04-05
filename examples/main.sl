main {
    foo: int = 4
    bar: int = foo + 2
    baz: int = bar * 2

    return (foo * foo + bar) - 2 * bar / (foo - 2) + baz * 2
}