# EK210_Room_Occupancy_Monitor
This is the code I wrote as a part of my EK210 (Engineering Design) team. The code is for a room occupancy monitor, which uses an Arduino Uno R3 to run, using 2 Infared Sensors, and a motor and buzzer are used as methods of alerting users that the room capacity has been reached. The project was done during the Spring 2024 semester.

This code checks for the amount of users that enter and exit a room, keeping track of a count. The user is allowed to set their own maximum room occupancy (Which maxes out at 100 --> Due to class requiring that being max), and once that capacity is breached, the buzzer will go off, making a loud noise to alert any other people that the room is at maximum capacity, whilst also spinning a motor which alerts users additionally using a flag. There are two sensors (both Infared), one of which is for users entering and exiting, doing the necessary reaction upon each condition being fulfilled. The code also displays the count to the user.



Code done by: Bogdan Sadikovic
Wiring done by: Henry Bega
Housing and Building done by: Brianna Vargas and Kantika (Andi) Klomjoho

Link to initial prototype demo: https://youtu.be/ay5nSJVjpag


