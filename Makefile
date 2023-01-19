all: cidrl cidrl6

cidrl: cidrl.c
	gcc -o cidrl cidrl.c -Wall

cidrl6: cidrl6.c
	gcc -o cidrl6 cidrl6.c -Wall -lm

clean:
	rm cidrl cidrl6

install:
	cp cidrl cidrl6 /usr/local/bin/
	mkdir --parents /usr/local/share/man/man1/
	cp cidrl.1 /usr/local/share/man/man1/
	cp cidrl6.1 /usr/local/share/man/man1/
