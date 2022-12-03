default: labirynt

%: %.cpp
	g++ -I. $< -o $@ Sphere.cpp Pyramid.cpp Cube.cpp  Camera.cpp -lGLEW  -lGL -lglfw 
clean:
	rm a.out *.o *~ labirynt
