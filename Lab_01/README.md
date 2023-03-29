##  Lab Report ##

What is something cool you did in this lab?
-----------
Your answer: I added the ability to use the onboard pushbutton to switch two LED modes. 
On startup the LED is off, one push of the button will turn on the LED into a blinking state, another press
will change the LED to a solid on state. Pressing the button again will swap it back to a blinking state,
then solid, etc.


Post-lab Questions
-------

* Answers to Question 1:
The circuit uses a capacitor in parallel with the pushbutton to filter out high frequency signals to produce less 
false positive button presses.

* Answers to Question 2:
With software, adding a delay of 20-50ms and checking again after what the processor thinks is a press to see if the
button was actually pressed. This method will add response time to the button press so a hardware solution is usually
more ideal.

* Answers to Question 3 (if appliable):
The pin for the pushbutton has already been pulled down externally so it does not require either a pull up or down
internally.

* Answers to Question 4 (if appliable):
Having an increased output speed increases energy usage and could introduce electromagnetic interferance
that could have unpredictable effects on neighboring circuits so if the application allows a slower speed that is more
ideal. The LED appears to the human eye turning on instantly at any speed so might as well get the benefit of reduced
EMI and power consumption by using the low speed.

Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?
I had the problem with missing some curly brackets {} in my program that took me a large portion of lab to figure out.
This is something that would have likely been resolved if lab mate collaboration was allowed. I do understand that this
could lead to one person figuring it out and just handing out a solution to everyone so then some don't achieve the
goal of the lab but it causes unneccessary frustration at times. This was the first lab so the TA was busy with all the 
first day lab duties. So it was difficult to ask them. If how I am understanding this rule is wrong it would be appreciated
to go over what is allowed and isn't in more detail. Thank you

* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?



Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: No.


