all:
	clang++ -std=c++11 -stdlib=libc++ main.cpp database.cpp matrixVectorLibrary.cpp polygons.cpp rayTracing.cpp -o rayTracer -framework GLUT -framework OpenGL

