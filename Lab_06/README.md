##  Lab Report ##

What is something cool you did in this lab?
-----------
Your answer: 
I rewired up the OLED display and gave it the ability to show the current status of the LED. Every time the button is pressed
it toggles that state of the LED and the output text on the display.

Post-lab Questions
-------

* Answers to Question 1:

To use software to clear the interrupt pending bit in the Pending Register (PR) of EXTI, you write a ‘1’ in the pending register. 
An example of this is "EXTI->PR1 |= EXTI_PR1_PIF13;"

* Answers to Question 2:

The program might not know if the interrupt has been completed and will continuously try to run it causing an infinite loop and or
unexpected/predictable behavior of the program.

* Answers to Question 3 (if appliable):

The bit can be set to a '1' so it can also be set to a '0' to trigger the interrupt. For example if you wanted software to trigger
the function. This should be avoided if possible though.

* Answers to Question 4 (if appliable):


Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?


* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?



Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: 


