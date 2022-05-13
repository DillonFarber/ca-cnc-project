# ca-cnc-project

### Folders
> - ca_projects_code - dir holds both the mian.cpp and the libs and resources that I used and uploaded to the Arduino IDE manage lib tabs
> - drawings - dir holds all the images that i used for the gcode. 
> - gcode - dir holds all gcode files for the project we tested the drawing cnc


### Tutorials 

#### Inkscape
> 1. Download inkscape tool from https://inkscape.org/ for your OS
> 2. The inkscape has all the gcode tools needed to create the gcode from an image. Tutorial I used form youtube https://www.youtube.com/watch?v=rMHTPXP7puo

#### Universal Gcode Sender
> 1. clone, download ugs form the https://winder.github.io/ugs_website/download/
> 2. connect arduino to usb
> 3. upload gcode to the usg platform
> 4. start ugs

#### Drawing CNC 
> clone my repo
> 1. modify the header to work for the Arduino IDE
> 2. download the Arduino IDE https://www.arduino.cc/en/software
> 3. copy and paste code into IDE
> 4. go to sketch->include libraries->add .ZIP library
> 5. find the folder of the resource folder from my repo and add them from there. read documents from main, you only need a few. The others are reference, and included in the Arduino IDE.


### Resource
- cnc layout for the project with pinouts and schematics https://schaltplan-online.blogspot.com/2022/01/arduino-cnc-shield-v3-schematic.html
- grbl software open source code used to test machine https://github.com/grbl/grbl 
