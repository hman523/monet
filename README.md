# Monet
A Basic Inspired Programming Language

Imagine a mix of BASIC, LISP, and whatever other language you can think of.
The more I work on this, the more I realize I am slowly implementing LISP.
In the words of Greenspun's tenth rule,
"Any sufficiently complicated C or Fortran program contains an ad-hoc, informally-specified, bug-ridden, slow implementation of half of Common Lisp."

This is my attempt at writing a language. While it will be a simple one, I am just doing this to learn and maybe make a useful scripting tool. Ultimately, I have no plans for this except to learn from it.

## Syntax
The syntax is similar to BASIC. Syntax is always 
`command parameter`. A function can have an arbitrary number of parameters. 
Commands and functions are case insensitive

Additionally, nesting is supported using parenthesis. For example:
`print (add 1 2)`


## Functions and subroutines

Functions allow for powerful abstractions and code reuse.
All function calls pass parameters by value (copy). Parameters must be typed.
Functions must have a return type and all functions must return something.
If you want a function to work like it is a void function, 
I recommend making a boolean and returning true if the function finishes.

To define a function, the following syntax is used.

```
define [return type] [function name] [parameter x type] [parameter x name]
[body]
[return statement]
end
```
For example:

```
define num triple num value
return (mul value 3)
end
```

To define a subroutine, the following syntax is used.
```
subroutine [name]
[body]
end
```
For example:

```
subroutine hello
println "Hello"
println "world"
end
```

So what's the difference between a subroutine and a function? 
Functions allow the user to pass values in and return a value. 
Subroutines on the other hand can't take parameters or return a value.
Why on earth would you use a subroutine if they are less feature full?
They have significantly less overhead and should be used for simple abstractions.
A good use of a subroutine would be printing a welcome message at the beginning of a program. 

Now what if you want to do some crazy deep recursion? 
Writing a simple Fibonacci function, we can see that as n increase, 
the time to compute grows. We can solve this with memoization.
Memoization essentially is the idea that if given a function that 
for any given input will always have the same output, 
we can store the output to optimize future calls. A little demo of this is shown below.

Here is a basic Fibonacci function.
```
define num fib num x
return (if (le x 1) x (add (fib (sub x 1)) (fib (sub x 2))))
end
```
When running this on my computer, it took 15.764 seconds to calculate (fib 30).
While not horrible, we can do better. By changing just one word,
 we can write the code like this
 ```
 defmem num fib num x
 return (if (le x 1) x (add (fib (sub x 1)) (fib (sub x 2))))
 end
 ```
 When running this solution, (fib 30) is able to run in .006 seconds! 
 While this optimization is nice, it comes at the cost of a memory overhead.

Built in commands so far:
`print`, `println`, `string`, `boolean`, `num`, `read`, `quit`, 
`add`, `sub`, `mul`, `div`, `not`, `and`, `or`, `nand`, `nor`, `xor`, `xnor`,
`if`, `eq`, `ne`, `gt`, `lt`, `ge`, `le`, `define`, `subroutine`, `defmem`

Commands coming soon: `list, cons, head, tail, map, reduce`

## If statements
If statements are very similar to those from Haskell. 
If you are familiar with that, you should feel right at home. 
For those who have not really worked with functional programming, 
the syntax goes something like this:

`if condition (return if true) (return if false)`

An example would be:

```
boolean a true 
num x (if (not a) (add 1 1) (sub 10 1))
println "x is " x
```
Output: `x is 9`

## Lists (note not released yet)
Lists are recursive containers that will be able to hold any datatype, including another list. 
These lists will be used for iteration and will be closely associated with the map and reduce functions. 
A list is said to be null if it is empty. A list can contain a mix of any datatype. 
A future feature should allow for functions to be held in a list.