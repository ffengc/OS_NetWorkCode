libhello.a:mymath.o myprint.o
	ar -rc libhello.a mymath.o myprint.o
mymath.o:mymath.c
	gcc -c mymath.c -o mymath.o
myprint.o:myprint.c
	gcc -c myprint.c -o myprint.o

.PHONY:hello
hello:
	mkdir -p hello/lib
	mkdir -p hello/include
	cp -rf *.h hello/include
	cp -rf *.a hello/lib

.PHONY:clean
clean:
	rm -f *.o libhello.a
	rm -rf hello