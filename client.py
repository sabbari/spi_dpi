#!/usr/bin/python3           # This is client.py file

import sys
import socket
import time
import codecs

import threading
import time

kbdInput = ''
finished = True
codecs.register_error("strict", codecs.ignore_errors)
tcpsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 

def kbdListener():
    global kbdInput, finished
    kbdInput = input("spi shell : ")
    # print ("maybe updating...the kbdInput variable is: {}".format(kbdInput))
    finished = True
def send_and_print(socket,string):
	if(len(string)):
		b=bytearray()
		b.extend(map(ord,string))
		b=b+bytearray([0xa])
		#print(b)
		k=socket.send(b)
		a=socket.recv(k)
		print(codecs.decode(a,'UTF-8'),end='',flush=True)
	



#num = (raw_input("Enter number: "))

tcpsocket.connect(('127.0.0.1', 1235)) 
i=0
while(1):
	k=tcpsocket.send(bytearray([0]))
	a=tcpsocket.recv(k)
	print(codecs.decode(a,'UTF-8'),end='',flush=True)
	if finished:
		send_and_print(tcpsocket,kbdInput)
		finished = False
		listener = threading.Thread(target=kbdListener)
		listener.start()
tcpsocket.close()
print(a)
print(k)


