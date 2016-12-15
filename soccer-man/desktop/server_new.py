import serial
import sys, tty, termios
import socketio
import eventlet
import json
import os
import time
import threading
import select

from multiprocessing import Process
# serial port consts
PORT_CONTROL = "/dev/cu.usbmodem1411"
PORT_TIME_OUT = 0.05

# action key bindings
KEY_LEFT = 'a'
KEY_RIGHT = 'd'
KEY_QUIT = 'q'
KEY_ATTACK = 'j'
KEY_FINISH = 'f'
KEY_RESTART = 'r'

# server init
mgr = socketio.KombuManager('redis://127.0.0.1:6379')
sio = socketio.Server(async_mode='eventlet', client_manager = mgr)
app = socketio.Middleware(sio)

# game classes
class Mock(object):

	def __init__(self):
		self.messages = ["p p1", "p p1", "p p2"]
		self.index = -1;

	def readline(self):
		self.index = self.index + 1
		i = self.index % 3

		return self.messages[i]

	def write(self, message):
		print "Writing: " + message

class Client(object):

	def __init__(self, port):
		
		self.ser = serial.Serial(PORT_CONTROL, 9600, timeout = PORT_TIME_OUT)
		#self.ser = Mock()

	def read(self):
		
		return self.ser.readline()

	def write(self, message):

		self.ser.write(message)


def isData():
    return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

old_settings = termios.tcgetattr(sys.stdin)

class Server(object):

	def __init__(self):
		
		self.client = Client(PORT_CONTROL)


	def get_ch(self):
		fd = sys.stdin.fileno()
		old_settings = termios.tcgetattr(fd)
		try:
			tty.setraw(sys.stdin.fileno())
			ch = sys.stdin.read(1)
		finally:
			termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

		return ch

	def parse_message(self, message):
		print message
		comps = message.split(' ')

		if len(comps) == 2:
			return comps[0], comps[1]

		return comps[0], comps[0]

	def init(self):

		self.client.write("READY\n")


	def receive_score_updates(self):

			# receive score updates
			message = str(self.client.read())
			
			if len(message) > 0:

				msg_type, msg_value = self.parse_message(message)

				if msg_type == 's':
					# multiplayer mode started
					sio.emit("started", data = json.dumps({ 'value' : msg_value }))

				if msg_type == 'f':
					# multiplayer mode finished
					sio.emit("finished", data = json.dumps({ 'value' : msg_value }));

				if msg_type == 'p':
					# player has scored
					sio.emit("score_increment", data = json.dumps({ 'player' : msg_value }))

	def detect_key_presses(self, ch):
			
			# detect key presses

			if ch == KEY_LEFT:
				self.client.write("L")
			elif ch == KEY_RIGHT:
				self.client.write("R")
			elif ch == KEY_ATTACK:
				self.client.write("S")
			elif ch == KEY_FINISH:
				self.client.write("F")
				sio.emit("finished", data = json.dumps({ 'value' : '1' }));
			elif ch == KEY_RESTART:
				self.client.write('|')
				sio.emit("score_reset")
			elif ch == KEY_QUIT:
				# terminal game
				self.client.write("Q")
				sio.emit("score_reset")
				# terminate redis server
				os.system("lsof -i tcp:6379 | grep redis-ser | grep -v grep | awk '{print $2}' | xargs kill")
				# terminate gunicorn server
				os.system("lsof -i tcp:8000 | grep Python | grep -v grep | awk '{print $2}' | xargs kill");
				# reset terminal settings
				termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)
				# clear mess
				print(chr(27) + "[2J")
				os.system('cls' if os.name == 'nt' else 'clear')
				# exit the code
				print "Quiting..."
				exit()

		
server = Server()

if __name__ == '__main__':

	# send heartbeat
	server.init()

	# start communicating
	try:
		tty.setcbreak(sys.stdin.fileno())

		while True:
			
			server.receive_score_updates()

			if isData():
				c = sys.stdin.read(1)
				server.detect_key_presses(c)
	finally:
		termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)
