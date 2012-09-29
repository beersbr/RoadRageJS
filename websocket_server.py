#!/usr/bin/python

import sys, os, math
from socket import *
from base64 import *
import time
from hashlib import sha1
import re
import struct

parser = re.compile("Sec-WebSocket-Key: (.+)")
magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

def create_accept_string(key):
	key = str(key).strip()
	print key
	new_string = key+magic
	s = sha1()	
	s.update(new_string)
	hsh = s.digest()
	hsh = hsh.strip()
	encoded_value = b64encode(hsh)
	print encoded_value
	return encoded_value

def get_header(new_key):
	websock_resp = "HTTP/1.1 101 Switching Protocols\r\n"
	websock_resp = websock_resp + "Upgrade: WebSocket\r\n"
	websock_resp = websock_resp + "Connection: Upgrade\r\n"
	websock_resp = websock_resp + "Sec-WebSocket-Origin: http://localhost\r\n"
	websock_resp = websock_resp + "Sec-WebSocket-Location: ws://localhost:9998/echo\r\n"
	websock_resp = websock_resp + "Sec-WebSocket-Accept: " + new_key + "\r\n"
	# websock_resp = websock_resp + "Sec-WebSocket-Protocol: chat\r\n"
	websock_resp = websock_resp + "\r\n\r\n"
	return websock_resp

# print websock_resp

listener = socket(AF_INET, SOCK_STREAM)
listener.bind(("localhost", 9998))
listener.listen(1)

client, address = listener.accept()
print "Connection made: ", address
try:
	resp = client.recv(512)
	print resp

	key = parser.search(resp)
	key = key.group(1)
	# print resp
	# print "KEY: ", key
	response = get_header(create_accept_string(key))

	print "RESPONSE:\n", response
	bits = client.send(response)

	print "SOMETHING: ", bits

	time.sleep(1)
	val = client.recv(512)
	print val.encode("hex")

	byte1 = ord(val[0])
	print byte1
	print "FIN: ", byte1 & 0x80
	print "rs1: ", byte1 & 0x40
	print "rs2: ", byte1 & 0x20
	print "rs3: ", byte1 & 0x10

	print "opc: ", byte1 & 0x0f

	byte2 = ord(val[1])
	print byte2
	print "mas: ", byte2 & 0x80
	print "siz: ", byte2 & 0x7f

	message_size = byte2 & 0x7f


	byteMask = val[2:6]
	
	message = val[6:(7+message_size)]

	cbyte = 0

	for byte in message:
		print byte, chr(ord(byte) ^ ord(byteMask[cbyte % 4]))
		cbyte += 1

except:
	client.close()
	listener.close()
	print "OOPS: ", sys.exc_info()[0]

client.close()
listener.close()