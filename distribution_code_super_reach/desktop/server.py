import serial
import sys, tty, termios
import socketio
import eventlet
import json
import os
import time

from multiprocessing import Process
# serial port consts
PORT_CONTROL = "/dev/cu.usbmodem1411"
PORT_TIME_OUT = 0.05

# action key bindings
KEY_LEFT = 'a'
KEY_RIGHT = 'd'
KEY_QUIT = 'q'
KEY_ATTACK = 'j'

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
					print "finished??"
					# multiplayer mode finished
					sio.emit("finished", data = json.dumps({ 'value' : msg_value }));

				if msg_type == 'p':
					# player has scored
					sio.emit("score_increment", data = json.dumps({ 'player' : msg_value }))

	def detect_key_presses(self):

			# detect key presses
			ch = self.get_ch()

			if ch == KEY_LEFT:
				self.client.write("L")
			elif ch == KEY_RIGHT:
				self.client.write("R")
			elif ch == KEY_ATTACK:
				self.client.write("S")
			elif ch == KEY_QUIT:
				# terminal game
				self.client.write("Q")
				sio.emit("score_reset")
				# terminate gunicorn server
				os.system("lsof -i tcp:8000 | grep Python | grep -v grep | awk '{print $2}' | xargs kill");
				# clear mess
				print(chr(27) + "[2J")
				os.system('cls' if os.name == 'nt' else 'clear')
				# exit the code
				print "Quiting..."
				exit()

	def start_communication(self):
		# detect score changes
		self.receive_score_updates()
		# detect key presses
		self.detect_key_presses()
		# spawn the socket on a separate process
		# p = Process(target=self.receive_score_updates)
		# p.start()
		


		
server = Server()

if __name__ == '__main__':

	sio.emit("started", data = json.dumps({ 'value' : '1' }))

	# send heartbeat
	server.init()

	# start communicating
	while True:
		server.start_communication()