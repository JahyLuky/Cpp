# Specifies the login name of the user who created this Makefile
LOGIN = chess

# Specifies the C++ compiler to use
CXX = g++

# Specifies basic compilation flags, including the C++ standard to use, optimization level, debugging information,
# undefined behavior sanitizer, and warnings to display
BASIC_FLAGS = -std=c++17 -O2 -g -fsanitize=undefined -Wall -pedantic

# Additional flags that can be specified at the command line, which will be passed to the compiler
FLAGS =

# List of files to include in the zip file created by 'make zip'
ZIP = Makefile Doxyfile DOCUMENTATION.md zadani.txt prohlaseni.txt \
  .gitignore $(wildcard examples/*) $(wildcard src/*)

# List of source files to compile
SOURCES = $(wildcard src/*.cpp)

# List of object files to generate during compilation, based on the list of source files
OBJECTS = $(patsubst src/%.cpp, build/%.o, ${SOURCES})

# List of dependency files to generate during compilation, based on the list of source files
DEPS = $(patsubst src/%.cpp, build/%.dep, ${SOURCES})

# Specifies phony targets, which do not correspond to actual files
.PHONY: all compile run valgrind doc clean count zip

# Default target, which compiles the program and generates documentation
all: compile doc

# Target to compile the program
compile: ${LOGIN}

# Rule to generate the executable program, which depends on the object files
${LOGIN}: ${OBJECTS}
	@mkdir -p build/
	${CXX} ${BASIC_FLAGS} ${FLAGS} $^ -o $@

# Rule to generate an object file from a source file
build/%.o: src/%.cpp
	@mkdir -p build/
	${CXX} ${BASIC_FLAGS} ${FLAGS} -c $< -o $@

# Target to run the program after compilation
run: compile
	./${LOGIN}

# Target to run the program with valgrind to detect memory errors
valgrind: compile
	valgrind ./${LOGIN}

# Target to generate documentation
doc: doc/index.html

# Rule to generate documentation using Doxygen, which depends on the Doxyfile, the DOCUMENTATION.md file,
# and all source files
doc/index.html: Doxyfile DOCUMENTATION.md $(wildcard src/*)
	doxygen Doxyfile

# Target to count the number of lines in all source files
count:
	wc -l src/*

# Target to remove all build and documentation files
clean:
	rm -rf build doc
	rm -f ${LOGIN} ${LOGIN}.zip

# Target to generate a zip file of all files necessary to build and run the program
zip: ${LOGIN}.zip

${LOGIN}.zip: ${ZIP}
	rm -rf tmp/
	rm -f $@
	mkdir -p tmp/${LOGIN}/
	cp --parents -r $^ tmp/${LOGIN}/
	cd tmp/ && zip -r ../$@ ${LOGIN}/
	rm -rf tmp/

# Rule to generate a dependency file from a source file
build/%.dep: src/%.cpp src/*
	@mkdir -p build/
	${CXX} -MM -MT $(patsubst src/%.cpp, build/%.o, $<) $< > $@

# Includes all dependency files
include ${DEPS}
