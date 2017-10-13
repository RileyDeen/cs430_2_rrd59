all: main.c 
	gcc main.c -o raycast.exe

clean:
	rm -rf raycast.exe *~