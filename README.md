# Artifice
Artifice is a simple functional programming language. Designed to be simple to read and write with.
Source files will compile down to LLVM intermediate code for further compilation. The goal for Artifice is
to be fast, reliable, and enjoyable to build with. Please consider checking out the project and contributing.

Documentation for getting started with the language can be found in the `langDocs/Artifice.pdf` document. The
language is under development so some things are subject to change over the course of the development of 
Artifice.

Check out the development Blog: [OneDev Development](https://noegarciaonedev.blogspot.com/)

## About the language
Artifice is a small functional programming language developed as a hobby project starting in 2023. Artifice is
built to be a bridge between imperative and declarative programming language domains. Built
to be simple and easy to read and write with, Artifice is a statically typed, semi-pure functional language
with basic support for simple tooling and functionality. While in its infant stages, the language will grow to
support more common aspects found in other languages.

Artifice is modeled off of other languages including C, Rust, and Elixir. As such, the language will look a little
familiar if you have used any of the aformentioned languages before. Artifice is named as such as it is a sanctuary
project.

## Current Progress
Artifice is in the very early stages of development. Current progress includes the lexer and the parser enabling the
compiler build an abstract syntax tree from the given file to compile. The next steps include populating symbol trees
with type information.

## Build and Run
Artifice relies on the `clang` compiler, along with the `Yacc` and `Flex` tools, all of which must be installed.

To compile Artifice, clone the source directory, and perform `make` within the main `Artifice/src` directory.
Currently only tested on Linux/Unix and Apple silicon systems.

To run the Artifice compiler, run the resulting object file called `solx` followed by the name of the source file to
be compiled.
~~~
$ ./solx example_file.solc
~~~

To run the debugger to print the abstract syntax tree of the compiled file to the console we set the flag.
~~~
$ ./solx -s example_file.solc
~~~

To print the symbols capured by the compiler during execution, set the symbol_table_flag during compilation.
~~~
$ ./solx -t example_file.solc
~~~