# Cell

## \*\***WORK IN PROGRESS**\*\*

An experiment to create a compiled programming language just for fun

```
use std;

printSum(a: int, b: int): int
{
    print a + b;
}

returnSum(a: int, b: int): int
{
    return a + b;
}

num: int = 5;
print "Hello, world!";

if num > 5
{
    print "num is greater than 5";
} 

elif num < 5
{
    print "num is less than 5";
}

else 
{
    print "num is equal to 5";
}

while num > 0 
{
    print num;
    num--;
}

for i: int = 0; i < 10; i++
{
    print i;
}

Breakfast: class
{
    init(meat: str, bread: str)
    {
        this.meat = meat;
        this.bread = bread;
    }

    serve(who: str) {
        print "Enjoy your " + this.meat + " and " + this.bread + ", " + who + ".";
    }
}

Brunch < Breakfast: class
{
    init(meat: str, bread: str, drink: str)
    {
        super.init(meat, bread);
        this.drink = drink;
    }
}
```

### Features

Data types:

```
int: integer (1, -48),
flo: floating point number (25.0, 31.31),
bool: boolean (tru, fls),
char: character ('c', 'o'),
str: string ("Hello world!")
void: void
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

```
minVal: int = min(5, 3);
printNewLine();
```

They are declared in the following manner:
```
printSum(a: int, b: int): void
{
    print a + b;
}
```

Classes follow this synntax
```
Breakfast: class
{
    init(meat: str, bread: str)
    {
        this.meat = meat;
        this.bread = bread;
    }

    serve(who: str) {
        print "Enjoy your " + this.meat + " and " + this.bread + ", " + who + ".";
    }
}
```

Instances of classes follow this syntax
```
baconAndToast: Breakfast("bacon", "toast");
copyOfBaconAndToast: Breakfast = baconAndToast;

baconAndToast.serve("Joe Biden");
```

A class that inherits properties from a parent class is declared like so
```
Brunch < Breakfast: class
{
    init(meat: str, bread: str, drink: str)
    {
        super.init(meat, bread);
        this.drink = drink;
    }
}
```