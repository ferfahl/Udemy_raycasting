NAME = raycast

FLAG = -lSDL2 -lm
##using the library that's installed on the system

build:
	cc -std=c99 -g3 ./src/*.c ${FLAG} -o ${NAME} `pkg-config --cflags --libs sdl2`
	# gcc -std=c99 -g3 ./src/*.c ${FLAG} -o ${NAME}

run:
	./${NAME}

clean:
	rm raycast