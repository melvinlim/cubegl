OUTPUT =		glcube
CXX =				em++
CXXFLAGS =	
LFLAGS =		-O3 -o $(OUTPUT).html --shell-file shell_minimal.html -s EXPORTED_FUNCTIONS='["_main","_checkbox","_setRotationSpeed","_setAngleY","_setAngleZ"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -s ASSERTIONS=1 -I/usr/local/include -std=c++11
OBJECTS =		glcube.cpp
all:		$(OBJECTS)
	$(CXX) $(LFLAGS) $(OBJECTS)
clean:
	rm $(OUTPUT).html $(OUTPUT).js $(OUTPUT).wasm
