=== Module Description ===
The accumulatron board is designed to be a communications hub for the casting robot. This 
board connects to a PC via USB and provides 5 RS-485 differential serial breakouts to slave 
modules (encoders, controllers, etc), a CAN interface, a wireless link to the string angle
encoder, and a few I/O lines for basic control. This board also contains a 9V preregulator
supply for external modules that can be disconnected. The board can be powered from USB for
testing/debugging, but for powering external devices like the spool encoder, the +9V supply
needs to be functioning or an alternative power source needs to be used.

All of the RS-485 serial lines are full duplex and utilize a 5-pin Molex Sherlock connector
with the 4 signal lines and a common reference. 
<Insert Pinout Here>

There is an Anaren A1109 radio module onboard to communicate with other wireless modules
such as the AirSSI board. This radio utilizes a TI proprietary protocol and communicates via
SPI to the STM32F103 on the Accumulatron board. Initialization software can be ported over 
from the AirSSI board AVR code.

There is one CAN interface on the board (untested) for a future upgrade path to replace
a separate USB-CAN adapater. This interface is designed to use an RJ-45 connector with a pinout
that matches the AMC DPCANTE motor amplifier connector.

== Manufacturing ==
The PCB CAM files for manufacturing are located in the Accumulatron_CAM folder. The following
Gerber files correspond to the following layers. All dimensions are in mils, the drill files
is a stanadard imperial unit Excellon drill file with 4 trailing zero formatting.
Top Copper
Bottom Copper
Top Soldermask		accumulatron_smt.GTS
Bottom Soldermask	accumulatron_smb.GBS
Top Silkscreen		accumulatron_silkt.GTO
Bottom Silkscreen 	accumulatron_silkb.GBO
Drill			ncdrill.txt

== Firmware ==
No completed firmware has been made for this board as testing was being done 
on the STM32F3/F4 discovery boards to get ChibiOS USB CDC/ACM drivers working. Once this 
board can act as a serial type device, the remainder of the firmware is just routing.

== Integration ==
**IMPORTANT**
When mounting this module, be sure that it is not inside of a metal housing or within 1" of 
a metal surface if the radio module is to be used.

=== Known Issues ===
- The primary 48V to 9V power supply was not functioning likely due to a damaged switching
regulator IC. This entire supply should be built up and tested
- The MCU was in the process of being replaced by Michael Anderson after a hardware failure
- The 48V power input connector is reverse polarity from the power supply wiring on the robot.
Do not plug directly into these, replace the PCB connector with a wired pigtail connector.
-The +9V peripheral load switch MOSFETs were not populated but should be in the electronics
bag.

There are spare unpopulated boards in the black filing cabinet in the orange DFRobot box

