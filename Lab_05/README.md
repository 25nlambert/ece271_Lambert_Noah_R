Pre-lab Questions
-------

* Answers to Question 1: The instruction used to call a function is BL (Branch and Link).

* Answers to Question 2: The instruction used to return a function is BX lr.

* Answers to Question 3 (if appliable): The register R0 is used to pass a single 32-bit integer argument to a function.

* Answers to Question 4 (if appliable): You need to store the value of LR otherwise it will be overwritten.

##  Lab Report ##

What is something cool you did in this lab?
-----------
Your answer: I implemented the button to control the motor. When the board starts up the onboard LED will flash. This indicates that the board is waiting for
the button to be pressed. Once the button is pressed it will rotate clockwise the specified degrees in the program. Once the motor has finished rotating it will
go back into the startup state of a blinking LED.


Post-lab Questions
-------

* Answers to Question 1:
An ABI is helpful for writing code so we can have better compatability with other boards and when functions get called to avoid having interferences.
The code is more likely to have a problem where memory is accessed or changed at the wrong time and create a non predicatable behavior.

* Answers to Question 2:
A great portion of the time, variables are used in one or two functions. When creating a global variable the takes up memory all the time which could lead to running
out of memory or a function modifies the variable when it shouldn't be. Once again creating unpredictable bahavior at times. This is not ideal. Storing
variables on the stack allows for more efficiency.

* Answers to Question 3 (if appliable):

* Answers to Question 4 (if appliable):


Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?


* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?



Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: 


