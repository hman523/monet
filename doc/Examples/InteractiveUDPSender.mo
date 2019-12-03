import "udp"

subroutine main
println (udp.receive 6789)
end

subroutine loop
main
loop
end

loop