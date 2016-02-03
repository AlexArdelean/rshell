# rshell
A shell designed to mimic basic vim functionality
=================================================

Features
--------

* execute the built in bash commands using execvp
* chain commands using ; && and ||

Limitations
-----------

* does not know how to use "cd" command
* 1000 char for input
* segmentation fault when command ends with ;
* overall weak bad-input checking
