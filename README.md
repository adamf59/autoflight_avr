# What is this project?

_AutoFlight_ is an extensible autopilot and control platform for aircraft research. The project is written in C++ and will support multiple AVR-based chips when finished.

## System Architecture

_AF_ is built around AVR-chips, like the ATmega 2560, ATmega 328P, and others. _AF_ supports a modular architecture, allowing for easy extension of the flight data network. More powerful SoCs (like the 2560) are designated as controllers, which can work in parallel with one another and compare results for redundancy. Additional SoCs (like the 328P) can be
added to serve as telemetry endpoints, register to recieve flight data from main processors, and add I/O for controlling hardware (i.e. landing gear). Information can be shared between processors using SPI or Serial.

## Acknowledgements
Code structure and some implementations are inspired by the ArduPilot project.
