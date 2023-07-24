##  Lab Report ##

Bonus Lab 2, Analog Clock with RTC

Lab Demo
-----------
Your answer: 
Displaying the time and AM/PM through an analog watch face on the OLED display using the RTC clock. 

A two functions were created, (RTC_Clock_Enable) one to enable the onboard RTC clock and connect it to the
LSE clock for the source. Another function (RTC_Init) was created to call the previous function and
generate a 1Hz signal along with setting the current time and date.

In the main function, RTC_Init is called to initialize the RTC. The forever while loop contains the calculations
to get the current time and whether it is AM or PM. There is a 24 hour to 12 hour conversion as the clock is only
12 hours. An outline for the clock is created by drawing 5 circles, each with a radius of 1 less. This produces a
thick outline. The hands are calculated with the math.h library, first the seconds, minutes, and hours values are
converted from degrees to radians for inputting into the trig functions. The coordinates are based off (cos,sin)
with offsets and scaling to create a watch face larger than 2 pixels tall. The first coordinate is always that
same, (64,32) which is the middle of the watch face. This is repeated for all three hands, with the hours hand has 
a reduction as a real clock would.

Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: 
No.

