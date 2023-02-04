all: cidrl cidrl6

cidrl: cidrl.c
	gcc -o cidrl cidrl.c -Wall

cidrl6: cidrl6.c in6calc.c in6calc.h
	gcc -o cidrl6 cidrl6.c in6calc.c -Wall

tags: in6calc.c
	ctags in6calc.c

clean:
	rm cidrl cidrl6

install:
	cp cidrl cidrl6 /usr/local/bin/
	mkdir --parents /usr/local/share/man/man1/
	cp man/* /usr/local/share/man/man1/
