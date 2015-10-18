all:
	clang++ raytracer.cpp defines.cpp main.cpp -lGL -lglut -std=c++11 -I. -o raytracer

