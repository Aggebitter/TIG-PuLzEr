# TIG-PuLzEr
Semi/Automatic TIG ARC trigger on Arduino

Video on function here https://youtu.be/d_q7jEKZYn8


             -----------   TIG PuLzEr ------------
I needed short on/off pulses on my tig at an adjustable pulse lenght both manual and automatic.
This is for welding very thin titanium sheets or other thin metals
  
 *  Usage:
The right nobb is multi function. Function select, Pulse time and Help
The left nobb sets the puls length for the arc
The Trigger activate the function
 
 *  Right nobb:
 
-Function select-
The left range sets the TIG PuLzEr in semi automatic mode called "TRIGGER"
and the arc has to be triggerd. 
The right range sets the TIG PuLzEr in autmatic mode called "PULSED"     
and the arc is automaticly triggerd. 
In the middle range it displays a simple help screen.

-Pulse time-
In the left and right function range it is still possible to adjust the 
length between arc pulses. 
 
-Help- 
To get a nice "space" between semi automatic and automatic mode, I 
put a gap in the range for tha potentiometer and why not use it for some
basic help. 

 * Left nobb:

Sets the lenght on each pulse. Notis that that the pulse lenght cant be 
longer than the pulse time and the software adjust it to be in range for
the pulse time

-Trigger-
When in semi automatic mode "TRIGGER", if the triggger pressed for a short time it 
will trigger a single pulse. If pressed for a long period it will trigger
pulses as long as the trigger is pressed.
In automatic mode "PULSED", if the triggger pressed for a short time it will start
generating pulses automatic. Holding the trigger for a long time will stop
the pulses. 
Stoping the pulses can also be done by turning the right nobb to  
ither Help or semi automatic mode "TRIGGER" mode.
 
The pulse lenght and puls time can be adjusted on the fly as long as you are in
the range for each function selected.


 *  To Do !! 
 
1, Well found a bug. In automatic mode "PULSED", pulse length can be higher than pulse time. 

2, Some testing while welding if I need to tweak some timings 

3, Use interupt to fetch trigger pressed events 
   It's is the only way to fix a realible exit from  automatic mode pulsed loop function
   Might be a way to do other stuff as showing a new help page

4, Hook it up to my osciloscope and measure the real timings compered to displayed
 
5, One stepper driver controller (to do a smoooth rotating welding lathe)
 
 6, pre and post gas asjustmens (for TIG's that lacks this feature) 
   Thats what the 3:rd and 4:th nobbs are for
 
 7, Tweak/Replace the Adafruit GFX... that one eats a lot of memory and we dont need much of it
  
 8, Find a way to detect torch contatc with the working pice so when lifted an arc is produced
    by ding it this way we can use the TIG as an Pulse Arc Welder 
    
   
   There should be some licensing stuff here but this is just a quick 'n dirty one 
   and I even forgot the adafruit license ... well its used and if you use my code add it ;-)
     
    //Agge

