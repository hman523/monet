import "f"

list a [1 2 3 4]

define num add_one num x
return (add 1 x)
end

define num sub_one num x
return (sub x 1)
end

list b [add_one sub_one add_one sub_one]

println "f.map add_one a = " (f.map add_one a)
println "f.reduce add a 0 = " (f.reduce add a 0)
println "f.apply b a " (f.apply b a)