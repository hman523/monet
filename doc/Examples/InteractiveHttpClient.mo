//We need to import the http library
import "http"

println "Welcome to the interactive HTTP Client"


//Here we define the main method. We have it return a number. If we wanted to make this program super fancy,
//we could test the return value but we don't since this program is only an example

define num main
//Prompt the user to input a command
println "What method would you like to use? (GET, POST, HEAD, PUT, DELETE)"
print "> "
read m

//prompt the user for a url
println "What url will you use?"
print "> "
read url

//Here's essentially an if, else if, else block.
println (if (eq m "GET") (http.get url) (if (eq m "POST") (http.post url) (if (eq m "HEAD") (http.head url) (if (eq m "PUT") (http.put url) (if (eq m "DELETE") (http.delete url) "Did not enter a valid method")))))

//print some whitespace
println ~ ~
return 0
end

//This is where the code loops via recursion, in a subroutine
subroutine loop
main
loop
end

loop