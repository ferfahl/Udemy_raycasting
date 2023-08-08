NAME = raycast

FLAG = -lSDL2 -lm
##using the library that's installed on the system

build:
	gcc -std=c99 -g3 ./src/*.c ${FLAG} -o ${NAME}

##cc -std=c99 -g3 ./src/*.c ${FLAG} -o ${NAME} `pkg-config --cflags --libs sdl2`
run:
	./${NAME}

clean:
	rm raycast