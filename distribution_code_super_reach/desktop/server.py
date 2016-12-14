import serial
import sys, tty, termios
import socketio
import eventlet
import json
import threading

# serial port consts
PORT_CONTROL = "/dev/cu.usbmodem1421"
PORT_TIME_OUT = 0.05

# action key bindings
KEY_LEFT = 'a'
KEY_RIGHT = 'd'
KEY_QUIT = 'q'
KEY_ATTACK = 'j'

# server address bindings
ADDR_SERVER = '127.0.0.1'
PORT_SERVER = 8888

# server init
mgr = socketio.KombuManager('redis://127.0.0.1:6379')
sio = socketio.Server(async_mode='eventlet', client_manager=mgr)
app = socketio.Middleware(sio)

# game classes
class Mock(object):

	def readline(self):
		print "Reading: EMPTY VALUE"
		return ""

	def write(self, message):
		print "Writing: " + message

class Client(object):

	def __init__(self, port):
		
		self.ser = serial.Serial(PORT_CONTROL, 9600, timeout = PORT_TIME_OUT)

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
		print message
		if len(comps) == 2:
			return comps[0], comps[1]
		return comps[0], comps[0]

		


	def init(self):

		self.client.write("READY\n")

	def start_communication(self):
		
		# detect received score update
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
				print "Scored!!"
				sio.emit("score_increment", data = json.dumps({ 'player' : msg_value }))

		# detect key presses
		while (True):
			ch = self.get_ch()
			if ch != '' : break

		if ch == KEY_LEFT:
			self.client.write("L")
			print "LEFT"
		elif ch == KEY_RIGHT:
			self.client.write("R")
		elif ch == KEY_ATTACK:
			self.client.write("S")
		elif ch == KEY_QUIT:
			self.client.write("Q")
			sio.emit("score_reset")
			print "Quiting..."
			exit()


server = Server()

if __name__ == '__main__':

	# send heartbeat
	server.init()

	# start communicating
	while True:
		server.start_communication()