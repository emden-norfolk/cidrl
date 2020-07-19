all: cidrl cidrl6

cidrl: cidrl.c
	gcc -o cidrl cidrl.c -Wall

cidrl6: cidrl6.c
	gcc -o cidrl6 cidrl6.c -Wall
