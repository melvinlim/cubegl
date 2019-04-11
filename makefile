OUTPUT =		glcube
CXX =				em++
CXXFLAGS =	
LFLAGS =		-O3 -o $(OUTPUT).html --shell-file shell_minimal.html -s EXPORTED_FUNCTIONS='["_main","_checkbox","_setDelay","_setAngles"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -s LEGACY_GL_EMULATION=1
OBJECTS =		glcube.c
all:		$(OBJECTS)
	$(CXX) $(LFLAGS) $(OBJECTS)
clean:
	rm $(OUTPUT).html $(OUTPUT).js $(OUTPUT).wasm
