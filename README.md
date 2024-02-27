# ESP32_SMeter
An LCD True Color SMeter for your CB using ESP32-2432S028 Board Kit

## Description
This project help you to build an LCD True Color SMeter for your CB (Midland, Galaxy, President, and more...) using a ESP32-2432S028 Board Kit and few other electronics components (OPAmp, resistors, trimmer, zener diode, etc.). Follow the schematics and the sketch it's very simple!

## Schematics
The signal out from the analog internal CB SMeter end enter into first OPAmp (LM358 dual OPAMP) as buffer. The signal out from first OPAmp end enter into second OPAmp as Gain (use VR1 to adjust meausrement). From the second OPAmp the signal pass throught a voltage limitators (R2 and DZ1 3.3v) and it enter into GPIO35 of ESP32-2432S028.

![alt text](https://github.com/StarNiell/ESP32_SMeter/blob/main/Schematics_ESP32_SMeter.jpg?raw=true)

## Notes
Use the VR1 (1M Trimmer) to adjust the output at about 3.2V - 3.3V whene the signal is set to 9+60: 
- The trick is to use another CB - one next to the other - on the same channel and press the push to talk.
  
The analog signals are not linear! Find the **ranlg[]** and **rsign[]** array variables and set the relative values (Linear interpolation):
- the trick is to read the analog data (via Serial.println()) at various levels of the signal and annotate the Analog Data in the **ranlg[]** array and the signal in **rsign[]** array with the same index.

![alt text](https://github.com/StarNiell/ESP32_SMeter/blob/main/Skecth_Notes.png?raw=true)
