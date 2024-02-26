# ESP32_SMeter
An LCD True Color SMeter for your CB using ESP32-2432S028 Board Kit

## Description
This project help you to build an LCD True Color SMeter for your CB (Midland, Galaxy, President, and more...) using a ESP32-2432S028 Board Kit and few other electronics components (OPAmp, resistors, trimmer, zener diode, etc.). Follow the schematics and the sketch it's very simple!

## Schematics
The signal out from the analog internal CB SMeter end enter into first OPAmp (LM358 dual OPAMP) as buffer. The signal out from first OPAmp end enter into second OPAmp as Gain (use VR1 to adjust meausrement). From the second OPAmp the signal pass throught a voltage limitators (R2 and DZ1 3.3v) and it enter into GPIO35 of ESP32-2432S028.

## Notes
The analog signal are not linear! Into the scketch find the **ranlg[]** and **rsign[]** array variables to set the relative values (Linear interpolation).

