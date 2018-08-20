all: main hello

main: main.c
	gcc -o main main.c -ldl -lcurl

hello: hello.c
	gcc -o hello.so -shared -fPIC hello.c

.PHONY: clean
clean:
	rm -f $(PROGRAMS)
