import "http"


subroutine main
println "Welcome to the http test file. This is just to show all the methods work as expected"

//first test the get method
gettest

//next test head
headertest

//next test post
posttest

//next test delete
deletetest

//next test put
puttest

//finally, show what happens if you fail to get a good response by sending a put when it expects post
badtest
//This one should cause an exception and fail the program

end

subroutine gettest
println "First, we're going to get a cat fact!" ~ ~ (http.get "https://catfact.ninja/fact") ~ ~ ~
end

subroutine headertest
println ~ ~ "That was really cool, but what else can this library do?" ~ "We can also get the header of a web page!"
println (http.head "https://www.google.com")
println ~ "Nice! It prints it as a list. There's a small issue currently in that the language can't handle escape"
println "sequences but most of this is proof of concept." ~ ~ ~
end

subroutine posttest
println "Now let's test the post method. To do this, I will send a message to a server that "
println "just sends a response that mirrors what I sent it. Let's say hi!"
println ~ "The server says " (http.post "http://httpbin.org/post?hi") ~ ~ ~
end

subroutine deletetest
println "To test delete, I'm using the same website but a different endpoint."
println "The response from the delete is " (http.delete "http://httpbin.org/delete") ~ ~ ~
end

subroutine puttest
println "Finally, to test put, I'm going to use my favorite website again" ~ (http.put "http://httpbin.org/put?hi") ~ ~
end

subroutine badtest
println "This next function call should cause the interpreter to crash. This is the desired outcome"
(http.put "http://httpbin.org/post?hi")
end

main