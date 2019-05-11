bin: main.cpp
	g++ -std=c++11   glad.c main.cpp sh.cpp genSamples.cpp initGL.cpp   -o bin -Wall  \
	-lglfw3 -lpthread -lGLEW -lGLU -lGL -lrt  -lXrandr -lXxf86vm -lXi \
	-lXinerama -lX11 -lXcursor -ldl -lSOIL -lassimp
