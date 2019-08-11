import time

def fib(x):
	return x if (x <= 1) else (fib(x-1)+fib(x-2))

x = 30
start = time.time()
print("Fib(" + str(x) + ") = " + str(fib(x)))
end = time.time()
print("Fib of " + str(x) + " took " + str(end-start) + "seconds to complete")
