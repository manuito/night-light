# night-light

A basic night-light for kids, made with D1 Mini and standard D1 Mini RGB Shield. Here is managed the arduino code for the D1 Mini

The supported features :

* A standard lamp switch as imput. The light stop but the wifi stay on for remote control
* Various light patterns supported. Can select with a REST API
* ON / OFF REST API entry point
* Alarm support. Get time by NTP. When alarm is ON, blink and beep with the embedded buzzer
* Remote control with a custom wifi button, made with D1 Mini lite, Battery shield + 400MaH battery, 1 button, 1 led + Resistor, in a small project box. Allows to switch the lamp on / off

## The lamp

Code is in ``arduino/lamp``. Details are on hackaday project

![Lamp](docs/lamp.jpg?raw=true "Lamp")

## The remote

It's basically an "internet button". Needs to be improved (Access to battery, adding a tilt switch for shack-based control of light pattern ...). 

![Button](docs/button.jpg?raw=true "Button")

Code is in ``arduino/remote``. Everything is IP based for now, I will work on a better access control for all my wifi projects another day ...