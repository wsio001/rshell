#Rshell bash
Rshell is a simple bash terminal emulator just like Unix bash terminal. For hw2, I added the test function, a specific hardcode "test", which means it is not base on execvp.

##Installation
* $ git clone https://github.com/wsio001/rshell.git
* $ cd rshell
* $ git checkout hw2
* $ make
* $ bin/rshell

##Rshell: Bugs and limitations

If connectors are next to each other and they are not the same, it will result in error. For example:
* "$ ls ||&& ": result error.
* "$ ls |& ": result error.

This bash emulator does not work properly when user used backspace.

Exit does not work as part of the command, I have to hardcode the command exit. It does not work with other command. For example:
* "$ exit" work just fine 
* while "$ exit || ls" does not work

It might not work properly when the command get too long.
For some reason, the strtok will add some weird character in some of the tokenized string, but most of them is fine.
