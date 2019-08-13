//This file is an example of a bunch of functions
//This was my primary test for a bunch of things early on
//It also has the file library function tests

defmem num fib num x
return (if (le x 1) x (add (fib (sub x 1)) (fib (sub x 2))))
end

define num apply fn func num x
return (func x)
end

define num oneandone fn func
return (func 1 1)
end

define num sum list lst
return (if (null lst) 0 (add (head lst) (sum (tail lst))))
end

define list range num start num finish
return (if (gt start finish) [] (cons start (range (add 1 start) finish)))
end


subroutine main
print ~ ~ ~
num x 1000
list y [(sub 1 1) 1 2 3]
println y
println "sum of y is " (sum y)
println (head y)
println (tail y)
println (cons 1 [1 2 3])
println (range 1 5)
println "fibonaci of x = " (fib x)
println "apply test " (apply fib 3)
println "one and one test " (oneandone mul)
import "file"
println (file.read "test1.txt")
list wordsinfile (file.words "test1.txt")
println wordsinfile
println (head wordsinfile)
println (file.count "test.mo")
//file.append "test1.txt" "println 5"
println "Last line of test1.txt: " (file.last "test1.txt")
end




main