default: labirynt

%: %.cpp
	g++ -I. $< -o $@ Sphere.cpp Pyramid.cpp -lGLEW  -lGL -lglfw 
clean:
	rm a.out *.o *~ labirynt
