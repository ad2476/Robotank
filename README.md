ROBOTANK
=======

- Controller: Operated by a human, this controller m2 sends commands to the robot.
- Brain: This is the m2 on the robot that does all the autonomous stuff, as well as processing commands from the controller.

------------

*About the protocol:*

Each packet is 4 bytes long and is structured like so: {MODE, byte1, byte2, checksum}. The two MODEs possible are DRIVE and FIRE. When in DRIVE mode, bytes 1 and 2 represent the left and right motor speeds, respectively. In FIRE mode, byte1 represents the tilt of the turret and byte2 carries the TRIGGER command. If byte2 carries the magic number 0xE7, the robot will fire once. The checksum at the end ensures that the packet was not heavily corrupted during transmission, or that a malicious packet was sent to spam the robot.

	NOTE: The checksum generated is not unique.



-------------

See medesign.seas.upenn.edu for more about the project, the m2, the mX board and SAAST.

NOTE: "saast.h" and the Makefile were created by J. Fiene for use with the m2 controller/mX board
