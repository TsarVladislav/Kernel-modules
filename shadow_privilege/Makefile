obj-m += kshadow2.o
obj-m += kshadow.o
FLAGS=-std=c89 -pedantic -Wall -Werror
all: ushadow
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

ushadow: ushadow.o
	gcc ushadow.o -o ushadow
ushadow.o: ushadow.c
	gcc -c ushadow.c
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean && rm ushadow
