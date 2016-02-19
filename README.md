#Rshell bash
Rshell is a simple bash terminal emulator just like Unix bash terminal.

##Installation
$ git clone https://github.com/wsio001/rshell.git
$ cd rshell
$ git checkout hw1
$ make
$ bin/rshell

##Rshell: Bugs and limitations

If connectors are next to each other and they are not the same, it will result in error. For example:
* "$ ls ||&& ": result error.
* "$ ls |& ": result error.

This bash emulator does not work properly when user used backspace.

Exit does not work as part of the command, I have to hardcode the command exit. It does not work with other command. For example:
* "$ exit" work just fine while "$ exit || ls" does not

Exit will not be working porperly when there is a command execvp fail. For how many times does the execvp fail, this is the amount of time that user has to enter "exit" to get out out the terminal. For example, if execvp failed 5 times before exit, then user will have to enter "exit" 5 times in order to get out from the terminal.

It might not work properly when the command get too long.
It create a Seg Fault at the end of a long command.
