import "udp"

define num handle_messaging
print "> "
read message
num port 6789
string ip "3.15.9.240"
udp.send ip port message
return 0
end

subroutine loop
handle_messaging
loop
end

define num main
num port 6789
string ip "3.15.9.240"
println "Enter Username: "
read username
println username
udp.send ip port username
println "waiting for response"
string token (udp.receive 6789)
println token
loop
return 0
end

main