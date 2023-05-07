# Solace
Solace is a simple functional programming language developed as a hobby project. Solace is
designed to be simple to read and write with. Source files will compile down
to assembly. The goal for Solace is to be fast, reliable, and enjoyable
to build with.

## About the language
Solace is a compiled functional programming language developed with the C
language utilizing Flex and Bison.
Solace is in its infancy and is a simple language. Solace is statically
typed with immutable variable types. The specification list and language
descriptions for Solace can be found in the langSpec/spec-file directory.

## Build and Run
Solace relies on the `clang` compiler, along with the Yacc and Flex tools,
all of which must be installed.

To compile Solace, clone the source directory, and perform `make` within
the main `Solace/src` directory.

To run the Solace compiler, run the resulting object file called `solace` 
followed by the name of the source file to be compiled.
~~~
$ solace example_file.solace
~~~