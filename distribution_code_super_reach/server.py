import serial

PORT_PLAYER_ONE = "/dev/cu.usbmodem1411"
PORT_PLAYER_TWO = "/dev/cu.usbmodem1421"

PORT_TIME_OUT = 0.1


class Player(object):

	def __init__(self, port):
		
		self.ser = serial.Serial(port, 9600, timeout = PORT_TIME_OUT)

	def read(self):
		
		return self.ser.readline()

	def write(self, message):

		self.ser.write(message)



class Server(object):

	def __init__(self):
		
		self.player1 = Player(PORT_PLAYER_ONE)
		self.player2 = Player(PORT_PLAYER_TWO)

	def startCommunication(self):
		
		while (True):
			
			p1msg = self.player1.read()

			p2msg = self.player2.read()

			if p1msg:
			 	
			 	#print "from player1: " + str(p1msg)
			 	self.player2.write(p1msg)

			if p2msg:
				
				#print "from player2: " + str(p2msg)
				self.player1.write(p2msg)


server = Server()

server.startCommunication();