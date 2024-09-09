# Solace
Solace is a simple functional programming language. Designed to be simple to read and write with.
Source files will compile down to LLVM intermediate code for further compilation. The goal for Solace is
to be fast, reliable, and enjoyable to build with. Please consider checking out the project and contributing.

Documentation for getting started with the language can be found in the `langDocs/SolaceLang.pdf` document. The
language is under development so some things are subject to change over the course of the development of 
Solace.

Check out the development Blog: [OneDev Development](https://noegarciaonedev.blogspot.com/)

## About the language
Solace is a small functional programming language developed as a hobby project starting in 2023. Solace is
built to be a bridge between imperative and declarative programming language domains. Built
to be simple and easy to read and write with, Solace is a statically typed, semi-pure functional language
with basic support for simple tooling and functionality. While in its infant stages, the language will grow to
support more common aspects found in other languages.

Solace is modeled off of other languages including C, Rust, and Elixir. As such, the language will look a little
familiar if you have used any of the aformentioned languages before. Solace is named as such as it is a sanctuary
project.

## Current Progress
Solace is in the very early stages of development. Current progress includes the lexer and the parser enabling the
compiler build an abstract syntax tree from the given file to compile. The next steps include populating symbol trees
with type information.

## Build and Run
Solace relies on the `clang` compiler, and is built entirely in the C programming Langauge.

To compile Solace, clone the source directory, and perform `make` within the main `Solace/src` directory.
Currently only tested on Linux/Unix and Apple silicon systems.

To run the Solace compiler, run the resulting object file called `anvl` followed by the name of the source file to
be compiled.
~~~
$ ./anvl example_file.arfc
~~~

To run the debugger to print the abstract syntax tree of the compiled file to the console we set the flag.
~~~
$ ./anvl -s example_file.arfc
~~~

To print the symbols capured by the compiler during execution, set the symbol_table_flag during compilation.
~~~
$ ./anvl -t example_file.arfc
~~~