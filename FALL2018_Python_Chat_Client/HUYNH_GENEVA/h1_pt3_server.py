import socket
import sys
import argparse
import threading
import re
import signal
import time

#Establishing command line arguments to be received
parser = argparse.ArgumentParser()
group = parser.add_argument_group()
group.add_argument("-p", "--port", help = "Please provide a port number after this flag.", type=int)
group.add_argument("-l", "--logfile", help = "Please provide the logfile to write to after this flag.", type=str)
parser.add_argument("-s", "--serveroverlayIP", help = "Please provide the server IP after this flag.", type=str)
parser.add_argument("-o", "--overlayport", help = "Please provide a server overlay port after this flag", type = int)
parser.add_argument("-t", "--serveroverlayport", help = "Please provide the server port after this flag.", type = int)
args = parser.parse_args()
#--------------------------------------------------------

#Check if no command line arguments have been provided, terminate program if so
if(not len(sys.argv) > 1 or not args.logfile or not args.port):
    print("\nCOMMAND LINE INSTRUCTIONS: ")
    print ("-p <port> port number the chat server")
    print ("-l <logfile> name of the logfile")
    print ("-s <serveroverlayIP> indicates the serverIP address")
    print ("-t <serveroverlayport> indicates the server port to connect to.")
    print ("-o <overlayport> indicates the server port used by other servers to be used to connect to this server.")
    print ("Must specify a logfile and port number before starting.")
    print ("During server operation, press Ctrl+C to terminate server.")
    print ("\nInsufficient arguments, terminating server now.")
    sys.exit()
# -------------------------------------------------------

#Assign command line arguments to variables
thread_lock = threading.Lock()
LOGFILE = args.logfile
PORT = args.port
if(args.serveroverlayIP):
    HOST = args.serveroverlayIP
else:
    HOST = 'localhost'
NEW_MESSAGE = 0
PARSE = []
if(args.overlayport):
    OVERLAY_PORT = args.overlayport
else:
    OVERLAY_PORT = None
if(args.serveroverlayport):
    SERVER_OVERLAY_PORT = args.serveroverlayport
else:
    SERVER_OVERLAY_PORT = None

#Open up writable to file
WRITE_TO_FILE = open(LOGFILE, "a+")
#--------------------------------------------------------
def broadcast(data):
#Given a message, sends it out to all client threads
    if len(client_connections) > 0:
        for users in client_connections:
            if data == "EXIT":
                break
            else:
                print('Broadcasting client port: %s' %(users.client_port))
                server_sock.sendto(data, (users.client_host, users.client_port))

        return 0
    else: 
        print('\nNo clients to broadcast messages to.')
        return 1
        
def direct_message(sender, dm_data, dm_address):
    server_sock.sendto('Received from %s: %s' %(sender, dm_data), dm_address)
    return 0

def transmit_message(sender, tm_data, tm_recipient):
    #send message to all servers in the list of servers
    for servers in server_connection:
        servers.conn.send('%s sentto %s %s' %(sender, tm_recipient, tm_data))
    return

#Define a server overlay thread, inheriting from thread class; thread deals with receiving messages from other servers
class overlay_server_thread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.daemon = True
        self.THREAD_LIVE = True
        self.TCP_server_sock = None
        self.TCP_server_conn = None
        self.TCP_server_address = None

    def run(self):
        #Create a TCP socket that will respond at the given port
        if not OVERLAY_PORT is None:
            #Create a TCP/IP socket and bind it to port
            self.TCP_server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            print('\nServer overlay starting up at port %s' %(OVERLAY_PORT))
            self.TCP_server_sock.bind((HOST, OVERLAY_PORT))
            WRITE_TO_FILE.write('\nServer overlay started at port %s' %(OVERLAY_PORT))
            #Become a server socket
            self.TCP_server_sock.listen(100)

            while 1:
                if self.THREAD_LIVE == False:
                    break

                #Receive server connections
                self.TCP_server_conn, self.TCP_server_address = self.TCP_server_sock.accept()
                #Print overlay server connection
                print ('Overlay server connection at %s, port %s' %(self.TCP_server_address[0], self.TCP_server_address[1]))
                self.TCP_server_conn.send('hello, welcome')
                #Create an individual thread for every connected overlay server
                server_thread = server_thread_receive(self.TCP_server_conn, self.TCP_server_address[0], self.TCP_server_address[1])
                server_connection.append(server_thread)
                server_thread.start()

class server_thread_receive(threading.Thread):
    def __init__(self, conn, host, port):
        threading.Thread.__init__(self)
        self.conn = conn
        self.host = host
        self.port = port
        self.THREAD_LIVE = True
        self.daemon = True
        self.receive = 0
        
    def run(self):
        global PARSE
        #self.conn.send('Welcome to server at host %s, port %s' %(HOST, OVERLAY_PORT))
        while 1:
            #Receive the data from the socket
            server_data = self.conn.recv(1024)

            if server_data == '1':
                self.receive = len(server_connection)+1
            elif server_data == '-1':
                self.receive = -1
            #Parse the data, check if the recipient is in this server
            SERVER_PARSE = server_data.split(" ")
            if len(SERVER_PARSE) > 3:
                sender = SERVER_PARSE[0]
                recipient = SERVER_PARSE[2]
                message = ""

                it = 3
                while it < len(SERVER_PARSE):
                    message += SERVER_PARSE[it]
                    if it < len(SERVER_PARSE)-1:
                        message += " "
                    it += 1
 
                USER_MATCH = False
                
                for users in client_connections: 
                    if users.client_name == recipient:
                        USER_MATCH = True
                        TARGET_HOST = users.client_host
                        TARGET_PORT = users.client_port

                if USER_MATCH == True:
                    #Use a DM to pass the message over to the client
                    self.conn.send('1')
                    direct_message(sender, message, (TARGET_HOST, TARGET_PORT))
                else:
                    self.conn.send('-1')

                #USER_MATCH = False

#Define client connection class, inheriting from thread class; thread deals with sending messages to client
class client_thread(threading.Thread):
    def __init__(self, socket, host, port, name):
        threading.Thread.__init__(self)
        #this client's personal instance of a socket
        self.client_socket = socket
        self.client_host = host
        self.client_port = port
        self.client_name = name
        self.daemon = True
        self.THREAD_LIVE = True
    
    def run(self):
        global NEW_MESSAGE
        global PARSE
        while 1:
            if not self.THREAD_LIVE:
                break
            thread_lock.acquire()
            if NEW_MESSAGE == 1:       
                #Check if this client thread's port matches incoming data's port
                if self.client_port == address[1]:
                    TARGET_MATCH = False
                    for users in client_connections:
                        if users.client_name == PARSE[2]:
                            TARGET_HOST = users.client_host
                            TARGET_PORT = users.client_port
                            TARGET_MATCH = True

                            DM = ""
                            i = 3
                            while i < len(PARSE) :
                                DM += PARSE[i]
                                if i < len(PARSE)-1:
                                    DM += " "
                                i += 1

                            NEW_MESSAGE = direct_message(new_client_name, DM, (TARGET_HOST, TARGET_PORT))
                            break
                    if(TARGET_MATCH == False and PARSE[1] == "sentto"):
                        
                        if(len(server_connection) > 0):
                            TM = ""
                            i = 3
                            while i < len(PARSE) :
                                TM += PARSE[i]
                                if i < len(PARSE)-1:
                                    TM += " "
                                i += 1
                            transmit_message(new_client_name, TM, PARSE[2])
                            time.sleep(0.5)

                            transmission = check_receive()
                            
                            while transmission >= ((len(server_connection))*(-1)) and transmission <= 0:
                                if(transmission <= ((len(server_connection))*-1)): 
                                    print('%s not registered with this server...' %(PARSE[2]))   
                                    WRITE_TO_FILE.write('\n%s not registered with this server...' %(PARSE[2]))
                                    break
                                elif(transmission > 0):
                                    #Message successfully received at another server
                                    print('Recipient found at another server!')
                                    break
                                else:
                                    print('Waiting for response at other servers...')
                                    time.sleep(0.5)
                        else:
                            print('%s not registered with this server...' %(PARSE[2]))  
                            WRITE_TO_FILE.write('\n%s not registered with this server...' %(PARSE[2]))

                    NEW_MESSAGE = 0
                    reset_receive()
                #If it is a match, parse the data for the destination and the message and direct message the recipient
            thread_lock.release()

def reset_receive():
    for servers in server_connection:
        servers.receive = 0

def check_receive():
    count = 0
    for servers in server_connection:
        count += servers.receive 

    return count    

#Handler for manual server termination  
class exit_thread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.daemon = True

    def run(self):
        while 1:
            try:
                pass
            except KeyboardInterrupt:
                print('Keyboard Interruption from exit_thread...')
                print('Terminating server...')
                WRITE_TO_FILE.write('Terminating server...')
                sys.exit()
#--------------------------------------------------------
#Create a handler for terminating the server (Ctrl+C)
thread_exit_handler = exit_thread()
thread_exit_handler.start()

#Create a UDP/IP socket
server_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = (HOST, PORT)

#Create thread to handle TCP connections
server_overlay = overlay_server_thread()
server_overlay.start()

#Create list to contain threads; new thread for each connection established by a client
client_connections = []

#Create list to contain server threads
server_connection = []

#Bind socket to port
print ('\nStarting up at address %s, port %s' %server_address)
server_sock.bind(server_address)
print ('\nSocket successfully bound! (Connection Protocol: UDP)')
WRITE_TO_FILE.write('\nServer started on %s at port %s' %server_address)

#If provided, connect to another server
if not SERVER_OVERLAY_PORT is None and not HOST is None:
    #Create a TCP/IP socket and bind it to port
    overlay_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print('Server overlay registration starting up at port %s' %(SERVER_OVERLAY_PORT))
    overlay_sock.connect((HOST, SERVER_OVERLAY_PORT))
    #WRITE_TO_FILE.write('\nServer overlay started at port %s' %(OVERLAY_PORT))

    s = server_thread_receive(overlay_sock, HOST, SERVER_OVERLAY_PORT)
    server_connection.append(s)
    s.start() 
    

#Waiting to read messages from socket
while True:

    try:
        thread_lock.acquire()
        print ('\nWaiting to receive message')
        #Program blocks until the clients sends a message over
        data, address = server_sock.recvfrom(4096)
        
        #Check if the data is a client termination signal -- remove client from list immediately if so
        if data == "EXIT":
            print("exit received")
            i = 0
            while i < len(client_connections):
                if(client_connections[i].client_port == address[1]):
                    DISCONNECT_CLIENT = i
                i += 1
            client_connections[DISCONNECT_CLIENT].THREAD_LIVE = False
            del client_connections[DISCONNECT_CLIENT]
            thread_lock.release()
            
        #Data is a normal message (DM or client registration message) -- parse the data, then add client to list/send message 
        else:
            thread_lock.release()
            #if the address has never been seen before, create a client thread for it and prepare to add it to list    
            #Creating the new thread
            PARSE = data.split(" ")
            new_client_name = PARSE[0]
            new_client_thread = client_thread(server_sock, address[0], address[1], new_client_name)
            #If this is the first client, add them to the list of clients
            if len(client_connections) == 0:
                WRITE_TO_FILE.write('\nClient connection from host %s port %s' %(address[0], address[1]))
                WRITE_TO_FILE.write('\nReceived register %s from host %s port %s' %(new_client_name, address[0], address[1]))
                print ('\nAdded FIRST client at %s, # of connections is now %s' %(address, len(client_connections)+1))
                #for servers in server_connection:
                    #transmit_message('\nAdded new client at %s' %(address))
                client_connections.append(new_client_thread)
                server_sock.sendto("welcome", (new_client_thread.client_host, new_client_thread.client_port))
                new_client_thread.start()
            #Checking for an existing match
            #    
            else: 
                CLIENT_EXISTS = False
                for client in client_connections:
                    #Iterate through clients in list, if there is a client that matches
                    if client.client_port == new_client_thread.client_port:
                        CLIENT_EXISTS = True

                if not CLIENT_EXISTS:
                    WRITE_TO_FILE.write('\nClient connection from host %s port %s' %(address[0], address[1]))
                    WRITE_TO_FILE.write('\nReceived register %s from host %s port %s' %(new_client_name, address[0], address[1]))
                    print ('\nAdded new client at %s, # of connections is now %s' %(address, len(client_connections)+1))
                    #for servers in server_connection:
                        #transmit_message('\nAdded new client at %s' %(address))
                    client_connections.append(new_client_thread)
                    server_sock.sendto("welcome", (new_client_thread.client_host, new_client_thread.client_port))
                    new_client_thread.start()

            WRITE_TO_FILE.write('\n%s' %(data))
            print ('Registered %s from host %s at port %s' %(new_client_name, address[0], address[1]))
            print (data)
            NEW_MESSAGE = 1

    except KeyboardInterrupt:
        print('Keyboard Interruption...')
        print('Terminating server...')
        WRITE_TO_FILE.write('\nTerminating server...')
        broadcast("exit")
        sys.exit()
        break
    
    except:
        print('System error, terminating server...')
        WRITE_TO_FILE.write('\nSystem error, terminating server...')
        sys.exit()
        break
    
#--------------------------------------------------------
