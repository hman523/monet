import "tcp"

println "Welcome to TCP Store Retrieve Service!"

define num main
println "Instructions:" ~ "store <key> <value>: stores the pair" ~ "retrieve <key>: retrieves the value for the key"
println ~
println "store or retrieve ?"
print "> "
read command
(if (eq command "store") (handle_store) (if (eq command "retrieve") (handle_retrieve) (if (eq command "quit") (println "quitting") (println "incorrect"))))
return 0
end

define bool send string message
num port 4445
string ip "3.15.9.240"
tcp.send ip port message
return true
end

define string receive
num port 4445
println (tcp.receive port)
end

define bool handle_store
send "store"
receive
println "key ?" ~ 
print "> "
read key
send key
receive
println "value ?" ~ 
print "> "
read value
send value
receive
return true
end

define bool handle_retrieve
send "retrieve"
receive
println "key ?" ~ 
print "> "
read key
send key
receive
return true
end

subroutine loop
main
loop
end

loop