CXX = g++
CPPFLAGS = -Wall -Werror -ansi -pedantic

RMDIR = rm -rf bin
MKDIR = mkdir bin


SRC = src

EXEC = rshell
SOURCECODE = rshell.cpp


all: rshell


rshell: 

	$(RMDIR)
	$(MKDIR)
	cd $(SRC) && $(CXX) $(CPPFLAGS) -o $(EXEC) $(SOURCECODE) && mv $(EXEC) ../bin
	
