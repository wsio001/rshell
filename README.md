#Rshell bash
Rshell is a simple bash terminal emulator just like Unix bash terminal.

##Installation
* $ git clone https://github.com/wsio001/rshell.git
* $ cd rshell
* $ git checkout hw1
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
It might create a Seg Fault at the end of a long command.
