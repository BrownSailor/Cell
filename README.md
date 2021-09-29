# Cell

An experiment to create a compiled programming language just for fun

```
main(argv: int, argc: []str): int {

    num: int = 5;
    print: "hello world!";

    // if-else statements
    ? num > 5 {
        print: 'num is greater than 5';
    } | num < 5 {
        print: 'num is less than 5';
    } | {
        print: 'num is equal to 5';
    }

    // for and while loops
    @ i: int = argc.sz() - 1, i >= 0, i-- {
        print: argc[i];
    }

    @ num > 0 {
        print: num;
        num--;
    }

    ret num;
}
```

### Features

Data types:

```
int: integer (1, -48),
flo: floating point number (25.0, 31.31,
bool: boolean (tru, fls),
char: character ('c', 'o'),
str: string ("Hello world!")
```

Variables are declared and initialized in the following manner

```
num: int;
num = 5;

text: string = "Welcome to Cell!";
```

There will be support for type inference, which will look like this

```
inferredInt = 5;
```

Functions are called by their name and the parameters passed inside parentheses `()`.
If no parameters are required, then the paraentheses are unnecessary

```
print(42, 45);
print;
```

If-Else statements are written with `?` and `|`

```
num: int = 5;

? num > 5 {
  print("Num is greater than 5");
} | num < 5 {
  print("Num is less than 5");
} | {
  print("Num is equal to 5");
}
```

For loops and while loops are written with the `@` symbol

```
@ c: char = 'a', c <= 'z', c++ {
  print(c);
}

i: int = 15;
@ i >= 0 {
  print(i);
  i--;
}
```

Function declaration

```
min3(a: int, b: int, c: int): int {

  ? a < b && a < c {
    ret a;
  } | b < a && b < c {
    ret b;
  }

  ret c;
}
```

Arrays are fixed size and created using the square brackets `[]` before the type name

```
arr: [4]int;

@ i: int = 0, i < 4, i++ {
  arr[i] = i * 2;
}
```

`arr` is now set to `[0, 2, 4, 6]`

Initializing at declaration is done using square brackets with the elements inside

```
arr: [3]int = [1, 2, 3];
```
