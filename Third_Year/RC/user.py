#!/usr/bin/python3
import socket   #for sockets
import sys  #for exit
import os

# ---- TCP socket

def createTCPsocket():
    """
        Creates the TCP socket and verifies if there is any error in it's creation.
    """
    try:
        return socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error:
        print('Failed to create TCP socket')
        sys.exit()

def receiveTCPmessage(clientSocket):
    """
        Receives the TCP message and verifies if there is any error in the reception.
    """
    try:
        return clientSocket.recv(1024)
    except socket.error:
        print('Failed to receive mensage through TCP socket')
        sys.exit()

def sendTCPmessage(clientSocket, message):
    """
        Argumets: The TCP socket and the message
        Sends the TCP message and verifies if there is any error in the reception.
    """
    try:
        return clientSocket.sendall(message.encode())
    except socket.error:
        print('Failed to send mensage through TCP socket')
        sys.exit()

# ---- UDP socket
def createUDPsocket():
    """
        Creates the UDP socket and verifies if there is any error in it's creation.
    """
    try:
        return socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    except socket.error:
        print('Failed to create UDP socket')
        sys.exit()

def sendUDPmessage(message, clientSocket, host, port):
    """
        Argumets: The message, UDP socket, host and the port
        Sends the UDP message and verifies if there is any error.
    """
    try:
        clientSocket.sendto(message.encode(), (host, port))
    except socket.gaierror:
        print("TCS hostname doesnt exist")
        sys.exit()
    except socket.error as msg:
        print('Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
        sys.exit()

def receiveUDPmessage(clientSocket):
    """
        Argumets: UDP socket
        Receives the UDP message, decodes it and verifies if there is any error
        in the reception.
    """
    clientSocket.settimeout(5)
    try:
        data, addr = clientSocket.recvfrom(1024)
        return data.decode()
    except socket.timeout:
        print("TCS-User UDP connection timed out. Check if TCS server is active")
        sys.exit()
    except socket.error as msg:
        print('Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
        sys.exit()

def getTCSargs():
    """
        Gets the arguments of when the user application is invoked, verifies if
        they correpond to an a machine name and a port.
    """
    try:
        nameIndex, portIndex = sys.argv.index("-n"), sys.argv.index("-p")
        if abs(nameIndex - portIndex) > 1:
            if isinstance(sys.argv[nameIndex+1],str) and isinstance(eval(sys.argv[portIndex+1]),int):
                return [sys.argv[nameIndex+1], int(sys.argv[portIndex+1])]
    except (ValueError, IndexError, NameError):
        return ["localhost", 58044]
    return ['localhost', 58044]

def checkTRR_NTA(dataBlock):
    return dataBlock[0:9].decode() == "TRR NTA\n"

def getTRRFileSize(dataBlock):
    """
        Aegument: dataBock - data recived through a socket
        Verifies the first 4 arguments given in the message and returns the size
        of the file.
    """
    if dataBlock[0:4].decode() == "TRR ":
        if dataBlock[4:6].decode() == "f ":
            for index in range(7, len(dataBlock)):
                if dataBlock[index:index+1].decode() == " ":
                    break
            filesize = ""
            for byte in range(index+1, len(dataBlock)):
                if dataBlock[byte:byte+1].decode() == " ":
                    return [int(filesize), byte+1]
                filesize += dataBlock[byte:byte+1].decode()
    return "random return string"


def processRequestCommand(command, languages):
    """
        Argumets: command and the language
        Processes the Request command, verifies the type of the translation and
        the if the command is valid.
    """
    command = command.split() #(request, language, t or f, words)
    try:
        translationType = command[2] # argumento t ou f do request
        numberLanguage = int(command[1]) #neste caso linguagem
        message = "UNQ "+ languages[numberLanguage-1] +"\n"
        if translationType == 't':
            numberWords = len(command[3:])
            if numberWords == 0:
                print("Bad format: no words to translate")
                return ["Wrong UNQ format\n",0,0]
            return [message, translationType, numberWords]
        elif translationType == 'f':
            filename = command[3]
            return [message, translationType, filename]
        else:
            return ["Wrong UNQ format\n",0,0]
    except IndexError:
        print('Request is not correctly formulated or you need to call the list command first')
        return ["Wrong UNQ format\n",0,0]
    except ValueError:
        #print('Wrong argument format\n')
        return ["Wrong UNQ format\n",0,0]

def isValidTCSresponse(response):
    """
        Argumets: response
        Verifies if the argument is valid, returns a boolean
    """
    if "UNR EOF" in response:
        print('Request cannot be answered, try change language')
        return False
    return True

def main():
    host, port = getTCSargs()
    udpSocket = createUDPsocket()
    print("TCS host: ", host, "\nTCS port: ", port)
    listOfLanguages = []

    while(1) :
        command = input('Enter command for client: ')
        if command[:4] == "list" and len(command) == 4:
            # verifies if the command recived request a list
            sendUDPmessage("ULQ\n",udpSocket , host, port)
            print("Message sent to TCS: ULQ\n")
            reply = receiveUDPmessage(udpSocket)
            if reply == "ULR EOF\n" or reply == "ULR ERR\n":
                print(reply)
            else:
                print(reply)
                reply = reply.split()
                listOfLanguages = []
                for i in range(2, len(reply)):
                    print(str(i-1) + " " + reply[i])
                    listOfLanguages.append(reply[i])

        elif command[:7] == "request":
            # verifies if the command recived request a request
            requestResult, translationType, specificData = processRequestCommand(command, listOfLanguages)
            if requestResult == "Wrong UNQ format\n":
                print(requestResult)
                continue
            sendUDPmessage(requestResult, udpSocket, host, port)
            print("Message sent to TCS: "+ requestResult)

            msgUDP = receiveUDPmessage(udpSocket)
            print("TCS UDP response: ", msgUDP)
            if isValidTCSresponse(msgUDP):
                token, ipTRS, portTRS = msgUDP.split()
                clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                try:
                    clientSocket.connect( (ipTRS, eval(portTRS)) )
                except ConnectionRefusedError as error:
                    print("The TRS server at ", ipTRS, "and port ", portTRS," is not active")
                    continue

                if translationType == 't':
                    #verifies if the command recived request for word translation
                    numberWords = specificData
                    listofWords = command.split()[3:]
                    message = "TRQ " + translationType + " " + str(numberWords) + " " + " ".join(listofWords) + "\n"
                    print("TRQ message: ", message)
                    sendTCPmessage(clientSocket, message)
                    reply = receiveTCPmessage(clientSocket)
                    print(reply.decode())
                    clientSocket.close()
                elif translationType == 'f':
                    #verifies if the command recived request for file translation
                    print("Received TRS info:", ipTRS, portTRS)
                    try:
                        filename = specificData
                        fileData = open(filename, mode ='rb').read()
                        message = 'TRQ '+ translationType + ' ' + filename + ' ' + str(len(fileData)) + ' '
                        message = message.encode() + fileData + "\n".encode()
                        clientSocket.sendall(message)
                    except FileNotFoundError:
                        print('File not found: ',filename)
                        continue

                    # read the first bytes to get translated filesize
                    firstBlock = clientSocket.recv(256)
                    if checkTRR_NTA(firstBlock):
                        print("TRR NTA\n")
                        clientSocket.close()
                        continue
                    filesize, lastIndex = getTRRFileSize(firstBlock)
                    print("Raw filesize: ", filesize)
                    translatedData = firstBlock[lastIndex::]
                    filesize = filesize - len(translatedData) - 1 #subtract final \n
                    print("Starting filesize: ",filesize)
                    while filesize > 0:
                        block_aux = clientSocket.recv(1024)
                        if len(block_aux) == 0:
                            print(0)
                        translatedData += block_aux
                        filesize -= len(block_aux)
                    translatedFileName, translatedFileSize = firstBlock.split()[2], firstBlock.split()[3]
                    open(translatedFileName, mode="wb").write(translatedData)
                    clientSocket.close()
                else:
                    print("Wrong user request syntax\n")
                    clientSocket.close()


        elif command[:4] == "exit" and len(command) == 4:
            print("Terminating client server...")
            sys.exit()

        else:
            print("Wrong command format: " + command)
            pass


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print("Terminar graciosamente o user...")
        sys.exit()
