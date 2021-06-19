import socket
import sys
import argparse
import threading
import re
import os

#Establishing command line arguments to be received
parser = argparse.ArgumentParser()
group = parser.add_argument_group()
group.add_argument("-p", "--port", help = "Please provide a port number after this flag.", type=int)
group.add_argument("-l", "--logfile", help = "Please provide the logfile to write to after this flag.", type=str)
parser.add_argument("-n", "--name", help = "Please provide your name after this flag.", type=str)
parser.add_argument("-s", "--serverIP", help = "Please provide the server IP after this flag.", type=str)
args = parser.parse_args()
#--------------------------------------------------------

#Check if no command line arguments have been provided, terminate program if so
if(not len(sys.argv) > 3):
    print("\nCOMMAND LINE INSTRUCTIONS: ")
    print ("-s <serverIP> indicates the serverIP address")
    print ("-p <port> port number for client to connect to server")
    print ("-l <logfile> name of the logfile")
    print ("-n <myname> indicates client name")
    print ("Must specify a logfile and port number before starting.")
    print ("During client operation, type exit to terminate client.")
    print ("\nInsufficient arguments, terminating client now.")
    sys.exit()
# -------------------------------------------------------

#Assign command line arguments to variables
CLIENT_ACTIVE = True
LOGFILE = args.logfile
PORT = args.port
if(args.serverIP):
    HOST = args.serverIP
else:
    HOST = 'localhost'
if(args.name):
    NAME = args.name

WRITE_TO_FILE = open(LOGFILE, "a+")
#--------------------------------------------------------

class client_thread(threading.Thread):
    def __init__(self, socket, address):
        threading.Thread.__init__(self)
        #This client's personal instance of a socket
        self.socket = socket
        self.address = address
        self.incoming_data = ""
        self.incoming_address = ""
        self.daemon = True

    def run(self):
        global CLIENT_ACTIVE
        while 1:
            #Listening for messages from server to print to user
            data, server = client_sock.recvfrom(4096)

            if(data == "exit"):
                print('Received exit command.')
                os._exit(0)

            WRITE_TO_FILE.write('\n%s' %(data))
            print (data)
#--------------------------------------------------------

# Create a UDP socket
client_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = (HOST, PORT)
print ('Connecting to the server %s at port %s...' %(HOST, PORT))
WRITE_TO_FILE.write("\nConnecting to the server %s at port %s, sending register message." %(HOST, PORT))
try: 
    sent = client_sock.sendto("%s registered from host %s, port %s." %(NAME, HOST, PORT), server_address)
    CONFIRM_MESSAGE, server = client_sock.recvfrom(4096)
    print(CONFIRM_MESSAGE)
    WRITE_TO_FILE.write("\nReceived %s" %(CONFIRM_MESSAGE))
except:
    print ('Server could not be reached!')
    print ('Terminating client...')
    sys.exit()
#--------------------------------------------------------

#Create client thread to receive messages
try:
    new_client_thread = client_thread(client_sock, server_address)
    new_client_thread.start()
except:
    print ('Client could not be started. Terminating program.')
    sys.exit()
#--------------------------------------------------------

while CLIENT_ACTIVE:
    try: 
        #Listen from input from user to send to server
        client_input = raw_input()

        if client_input == "exit":
            sent = client_sock.sendto("EXIT", server_address)
            CLIENT_ACTIVE = False             
            client_sock.close()        
            sys.exit() 
        
        MESSAGE = ""
        INPUT_ERROR = True

        parse = client_input.split(" ")
        
        if(parse[0] == "sendto"):

            if(parse[1]):
                INPUT_ERROR = False
                DESTINATION = parse[1]   
                i = 2
                while i < len(parse) :
                    MESSAGE += parse[i]
                    if i < len(parse)-1:
                        MESSAGE += " "
                    i += 1

            else: 
                print('Error! Command requires a destination parameter.')
        else:
            print('Error! Command requires <sendto> flag.')

        if INPUT_ERROR == False:
            # Send data
            sent = client_sock.sendto("%s sentto %s %s" %(NAME, DESTINATION, MESSAGE), server_address)
            WRITE_TO_FILE.write('\n%s' %(client_input))
        
            
    except:
        print ('Connection to server has been closed!')
        print ('Terminating client...')
        WRITE_TO_FILE.write('\nTerminating client...')
        sys.exit()
#---------------------------------------------------------

