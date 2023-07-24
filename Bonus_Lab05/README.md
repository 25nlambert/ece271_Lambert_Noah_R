##  Lab Report ##

Bonus Lab 5, RTC

Lab Demo
-----------
Your answer: 
Displaying the time, date, and weekday on the OLED display using the RTC clock. 

Two functions were created, (RTC_Clock_Enable) one to enable the onboard RTC clock and connect it to the
LSE clock for the source. Another function (RTC_Init) was created to call the previous function and
generate a 1Hz signal along with setting the current time and date.

In the main function, RTC_Init is called to initialize the RTC. The forever while loop contains the calculations
to get the current time, date, and weekday from the Date and Time registers through shifting and masking. There 
is a switch case to get a string that contains the weekday, at the end are function calls for the display to 
show the time, date, and weekday in decending order on the OLED display.

Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?


* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?



Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: 


