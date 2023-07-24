##  Lab Report ##

Bonus Lab 1, Buzzer

Lab Demo
-----------
Your answer: 
Using a buzzer to play a song by varying the frequency at which the buzzer oscillates and how long each
note is played for.

The program has two arrays that hold the length of time each note needs to be played for in ms and another
contains the frequency at which each note should be played at. The enters the forever loop and iterates
through a for loop for each note that needs to be played. The 1/2 period of the frequency wave is calculated
then the ratio between the 1/2 period and the smallest delay time for the input of the delay for oscillation
of the buzzer. Using the current note value, the amount of times the buzzer needs to oscillate to play for
the correct duration is calculated by multiplying it by a scaler to keep the time values at the same ratio
for integer division and the value is divided by the value of 250ms. A for loop the controls the buzzer is
run next that takes in the amount of loops the oscillations need to occur. This loop is followed by a delay
to add definition to the tones being produced. At the end, a delay is added for once the song is done to
pause so that the end and start of each playthtough.

Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: 
No.

