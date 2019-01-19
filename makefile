release:
	gcc -o diceware -std=c11 -Os main.c

clean:
	rm diceware

install:
	sudo bash install.sh

all: release install clean

