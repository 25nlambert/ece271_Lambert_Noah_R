##  Lab Report ##

What is something cool you did in this lab?
-----------
Your answer:  I added the ability to use the onboard pushbutton to switch two LED modes.
On startup the LED is off, one push of the button will turn on the LED into a blinking state, another press
will change the LED to a solid on state. Pressing the button again will swap it back to a blinking state,
then solid, etc.


Post-lab Questions
-------

* Answers to Question 1: 
The memory set to hold the value of the immediate is only 8 bits. A 32 bit immediate is too big.

* Answers to Question 2:
Get an error that the value is not between 0 and 255. You can get arounf this by using the MOV instruction for the top bits and
LDR for the lower half of the bits if you need something larger.

* Answers to Question 3 (if appliable):
CMP only changes the flags and leaves the value untouched
SUB will change the flags and will modify the value.

* Answers to Question 4 (if appliable):
Yes, it will clear the bit which will bring the pin output to low, shutting off the LED.
BIC r0, r0, #GPIO_ODR_ODR_5

Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?
No

* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?
No


Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: No


