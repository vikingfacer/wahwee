
# Wahwee
Wahwee is a robot from scratch project. Aiming at reproducibility. This repo only contains the software for the project. Wahwee is more than just software, Building the battery, power bus and robot frame are also big components of this project but currently not posted.
## Joystick Server
The Joystick controller reads the input from a joystick controller Xbox, PS\<__whatever__> and publishes commands that are consumed but the listener

## Pin Controller
The Pin Controller/py-controller subscripts the the Joystick Server and updates the outputs and inputs of the RPi pins.

## How it runs
* Pin Controller RPi acts as wifi AP
* Joystick Server connects as wifi client
* Joystick Server remotely starts Pin Controller with unique subscription
* Joystick To Controller communications Commence

## Road Map
[x] Basic Controlling of Motors
[x] RPi wifi AP
[  ] unique Subscription
[  ] Script for Pin Controller
[  ] Camera support (Just using RPi motion)
[  ] custom Linux image recipe for Pin Controller and other robot services
[  ] Robot Arm Support
