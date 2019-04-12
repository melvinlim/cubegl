OUTPUT =		glcube
CXX =				em++
CXXFLAGS =	
LFLAGS =		-O3 -o $(OUTPUT).html --shell-file shell_minimal.html -s EXPORTED_FUNCTIONS='["_main","_checkbox","_setDelay","_setAngles"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -s ASSERTIONS=1 -I/usr/local/include
OBJECTS =		glcube.cpp
all:		$(OBJECTS)
	$(CXX) $(LFLAGS) $(OBJECTS)
clean:
	rm $(OUTPUT).html $(OUTPUT).js $(OUTPUT).wasm
