default: labirynt

%: %.cpp
	g++ -I. $< -o $@ Pyramid.cpp -lGLEW  -lGL -lglfw 
clean:
	rm a.out *.o *~ labirynt
