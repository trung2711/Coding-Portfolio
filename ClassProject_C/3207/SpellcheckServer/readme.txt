Multithreaded Spellcheck Server

Credits to author and programmer Micheal Kerrisk for the input checking methods.
Huge credits to professor John Fiore at Temple University for this program's functionality.

I. Description

Spellcheck is a program that use multithreading and server-client connection to check the spelling of user inputs. The program comes with a default library file (.txt), but can be replaced for deeper spellcheck.

II. To excecute the program

To thoroughly test the program, open the program using one computer, then use different computers at the same time to connect and test the spellchecking.

To test this program without using a second computer, copy everything to your folder, then open the spellchecker in background mode (Try "./spellchecker &"). Then, while still in your terminal, type "telnet 127.0.0.1 11111" to self-connect to the server (127.0.0.1 is the loopback address, 11111 is the default port number stored in the file). Note that the multithread functionality of the program cannot be tested using this method. It is just a quick way to check the spellcheck functionality.

III. Functionality

When opened and connected, the program will return the word you typed, along with the message "OK" if it is in the given dictionary file and "MISPELLED" if it is not.

IV. Possible improvements

Splitting up the file can be a good idea!