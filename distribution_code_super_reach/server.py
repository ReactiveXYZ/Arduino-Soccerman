import serial

import sys, tty, termios

PORT_CONTROL = "/dev/cu.usbmodem1411"
PORT_TIME_OUT = 0.1

KEY_LEFT = 'a'
KEY_RIGHT = 'd'
KEY_QUIT = 'q'
KEY_ATTACK = 'j'

class Client(object):

	def __init__(self, port):
		
		self.ser = serial.Serial(port, 9600, timeout = PORT_TIME_OUT)

	def read(self):
		
		return self.ser.readline()

	def write(self, message):

		self.ser.write(message)



class Server(object):

	def __init__(self):
		
		self.client = Client(PORT_CONTROL)
		pass

	def get_ch(self):
		fd = sys.stdin.fileno()
		old_settings = termios.tcgetattr(fd)
		try:
			tty.setraw(sys.stdin.fileno())
			ch = sys.stdin.read(1)
		finally:
			termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
		return ch

	def startCommunication(self):
		
		while (True):
			# detected key presses
			ch = self.get_ch()
			if ch != '' : break
		if ch == KEY_LEFT:
			self.client.write("L")
		elif ch == KEY_RIGHT:
			self.client.write("R")
		elif ch == KEY_ATTACK:
			self.client.write("S")
		elif ch == KEY_QUIT:
			print "Quiting..."
			exit()


server = Server()

if __name__ == '__main__':
	while True:
		server.startCommunication();