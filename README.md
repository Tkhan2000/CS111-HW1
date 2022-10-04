# Pipe Up

This lab is a C implementation of the piping operator used in
Linux (i.e - ls | cat | wc).

## Building

The program is built by going into the lab1 path in terminal and
running the "make" command. It will build binary files and the
pipe executable.

## Running

The program is run by using the command "./pipe" and listing linux 
commands as agruments to be run and piped sequentially. An example
of this is "./pipe ls cat wc", which functions exactly like the linux
command "ls | cat | wc".

## Cleaning up

While in the lab1 directory, run the command "make clean" to
automatically delete the binary files and the pipe executable.