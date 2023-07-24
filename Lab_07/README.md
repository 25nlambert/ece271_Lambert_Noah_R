##  Lab Report ##

Lab Demo
-----------
Your answer: 
Oscilliscope reading of LED toggling every 1 second.
![Osc](/Lab_07/images/osc.png)

The timing is very accurate, right on the head of 2S period, this is on and off time so divide by two to get exactly
1 second on and 1 second off.

The percent error is 0%. For the decimal places that the Analog Discovery 2 can measure it reads an exact 2.0000S.

What is something cool you did in this lab?
-----------
Your answer: I added controls to modify the delay when the LED blinks. Additionally I also added, each number button except 0, 
blinks the LED the corresponding amount of times.

Post-lab Questions
-------

* Answers to Question 1:

SysTick is defined at interrupt number 15 defined in CMSIS.

* Answers to Question 2:

Default priority is 0. A smaller number means higher urgency, so a higher priority value does not repreesent a higher priority.

* Answers to Question 3 (if appliable):

No, this functionality is required to be enabled.

* Answers to Question 4 (if appliable):

1.04Seconds is the maximum interval. This is calculated with the 24bit SysTick register * the period.

Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?


* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?



Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: 


