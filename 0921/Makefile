.PHONY:all
all: exec mycmd 

exec:exec.c
	gcc -o $@ $^
mycmd:mycmd.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f exec mycmd 
