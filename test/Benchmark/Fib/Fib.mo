define num fib num x
return (if (le x 1) x (add (fib (sub x 1)) (fib (sub x 2))))
end


subroutine main
import "time"
num start_time (time.time)
num x 30
println "Fib(" x ") = " (fib x)
num end_time (time.time)
println "Fibonacci of " x " took " (sub end_time start_time) " seconds to complete"
end

main