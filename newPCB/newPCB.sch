EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:newPCB-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_01X10 P2
U 1 1 580E5282
P 4200 2950
F 0 "P2" H 4200 3500 50  0000 C CNN
F 1 "CONN_01X10" V 4300 2950 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x10" H 4200 2950 50  0001 C CNN
F 3 "" H 4200 2950 50  0000 C CNN
	1    4200 2950
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X10 P3
U 1 1 580E52A7
P 5050 2950
F 0 "P3" H 5050 3500 50  0000 C CNN
F 1 "CONN_01X10" V 5150 2950 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x10" H 5050 2950 50  0001 C CNN
F 3 "" H 5050 2950 50  0000 C CNN
	1    5050 2950
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P1
U 1 1 580E52D6
P 3150 3050
F 0 "P1" H 3150 3300 50  0000 C CNN
F 1 "CONN_01X04" V 3250 3050 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04" H 3150 3050 50  0001 C CNN
F 3 "" H 3150 3050 50  0000 C CNN
	1    3150 3050
	1    0    0    -1  
$EndComp
Text GLabel 4000 2500 0    39   Input ~ 0
GND
Text GLabel 4850 2500 0    39   Input ~ 0
GND
Text GLabel 2950 2900 0    39   Input ~ 0
GND
Text GLabel 4000 2700 0    39   Input ~ 0
LED-V
Text GLabel 2950 3200 0    39   Input ~ 0
LED-V
Text GLabel 4000 3200 0    39   Input ~ 0
pin5_clock
Text GLabel 4000 3100 0    39   Input ~ 0
pin4_data
Text GLabel 2950 3100 0    39   Input ~ 0
pin5_clock
Text GLabel 2950 3000 0    39   Input ~ 0
pin4_data
Text GLabel 4850 2700 0    39   Input ~ 0
v+
Text GLabel 4850 2900 0    39   Input ~ 0
pin12_btn2
Text GLabel 4850 3000 0    39   Input ~ 0
pin14_btn1
$Comp
L SW_PUSH SW1
U 1 1 580E5697
P 6200 3000
F 0 "SW1" H 6350 3110 50  0000 C CNN
F 1 "SW_PUSH" H 6200 2920 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH-12mm" H 6200 3000 50  0001 C CNN
F 3 "" H 6200 3000 50  0000 C CNN
	1    6200 3000
	-1   0    0    1   
$EndComp
$Comp
L SW_PUSH SW2
U 1 1 580E56C9
P 6200 2700
F 0 "SW2" H 6350 2810 50  0000 C CNN
F 1 "SW_PUSH" H 6200 2620 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH-12mm" H 6200 2700 50  0001 C CNN
F 3 "" H 6200 2700 50  0000 C CNN
	1    6200 2700
	-1   0    0    1   
$EndComp
Text GLabel 6500 3000 2    39   Input ~ 0
v+
Text GLabel 6500 2700 2    39   Input ~ 0
v+
Text GLabel 5650 3000 0    39   Input ~ 0
pin14_btn1
Text GLabel 5650 2700 0    39   Input ~ 0
pin12_btn2
$Comp
L R R1
U 1 1 580E848F
P 5900 2300
F 0 "R1" V 5980 2300 50  0000 C CNN
F 1 "10K" V 5900 2300 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM7mm" V 5830 2300 39  0001 C CNN
F 3 "" H 5900 2300 50  0000 C CNN
	1    5900 2300
	-1   0    0    1   
$EndComp
$Comp
L R R2
U 1 1 580E84FB
P 5900 3400
F 0 "R2" V 5980 3400 50  0000 C CNN
F 1 "10K" V 5900 3400 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM7mm" V 5830 3400 39  0001 C CNN
F 3 "" H 5900 3400 50  0000 C CNN
	1    5900 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 580E8A62
P 5900 2000
F 0 "#PWR01" H 5900 1750 50  0001 C CNN
F 1 "GND" H 5900 1850 50  0000 C CNN
F 2 "" H 5900 2000 50  0000 C CNN
F 3 "" H 5900 2000 50  0000 C CNN
	1    5900 2000
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR02
U 1 1 580E8C32
P 5900 3650
F 0 "#PWR02" H 5900 3400 50  0001 C CNN
F 1 "GND" H 5900 3500 50  0000 C CNN
F 2 "" H 5900 3650 50  0000 C CNN
F 3 "" H 5900 3650 50  0000 C CNN
	1    5900 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 2450 5900 2700
Wire Wire Line
	5900 3250 5900 3000
Wire Wire Line
	5900 2700 5650 2700
Wire Wire Line
	5900 3000 5650 3000
Wire Wire Line
	5900 3550 5900 3650
Wire Wire Line
	5900 2000 5900 2150
$EndSCHEMATC
