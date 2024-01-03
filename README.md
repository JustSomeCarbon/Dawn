# Solace
Solace is a simple functional programming language. Designed to be simple to read and write with.
Source files will compile down to LLVM intermediate code for further compilation. The goal for Solace is
to be fast, reliable, and enjoyable to build with. Please consider checking out the project and contributing.

Documentation for getting started with the language can be found in the `langDocs/Solace.pdf` document. The
language is under development so some things are subject to change over the course of the development of 
Solace.

Check out the development Blog: [OneDev Development](https://noegarciaonedev.blogspot.com/)

## About the language
Solace is a small functional programming language developed as a hobby project starting in 2023. Solace is
built to be a bridge between imperative and declarative functional programming language domains. Built
to be simple and easy to read and write with, Solace is a statically typed, semi-pure functional language
with basic support for simple tooling and functionality. While in its infant stages, the language will grow to
support more common aspects found in other languages.

Solace is modeled off of other languages including C, Rust, and Elixir. As such, the language will look a little
familiar if you have used any of the aformentioned languages before. Solace is named as such as it is a sanctuary
project.

## Current Progress
Solace is in the very early stages of development. Current progress includes the lexer and the parser enabling the
compiler build an abstract syntax tree from the given file to compile. 

## Build and Run
Solace relies on the `clang` compiler, along with the `Yacc` and `Flex` tools, all of which must be installed.

To compile Solace, clone the source directory, and perform `make` within the main `Solace/src` directory.
Currently only tested on Linux/Unix and Apple silicon systems.

To run the Solace compiler, run the resulting object file called `solx` followed by the name of the source file to
be compiled.
~~~
$ ./solx example_file.solc
~~~
