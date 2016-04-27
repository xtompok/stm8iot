#!/usr/bin/python2

import serial
import sys

types = {1:"player",2:"city",3:"source",4:"base"}
modes = {1:"attack",2:"defense",3:"auto"}
actions = {1:"broadcast",2:"attack",3:"transfer",4:"source"}
stages = {0x00: "-",0x10:"init", 0x20:"inack", 0x30:"do", 0x40:"done"}

if len(sys.argv)==1:
	print "No port defined"
	sys.exit(1)

ser = serial.Serial(sys.argv[1],115200)
while True:
	line = ser.readline()[5:-2]
	data = map(lambda x: int(x,0),line.split(" "))
	typ = types[data[6]]
	action = actions[data[9] & 0x0F]
	stage = stages[data[9] & 0xF0]
	teams = data[10:20]
	times = data[20:30]
	print 'From:{0[0]:3} to:{0[1]:3} type:{1:6} team:{0[7]:3} units:{0[8]:3} action:{2:10} stage:{3:5}'.format(data,typ,action,stage)
	print 'History: {0[0]}:{1[0]} {0[1]}:{1[1]} {0[2]}:{1[2]} {0[3]}:{1[3]} {0[4]}:{1[4]} {0[5]}:{1[5]} {0[6]}:{1[6]} {0[7]}:{1[7]} {0[8]}:{1[8]} {0[9]}:{1[9]}'.format(times,teams) 
	
	
