#!/usr/bin/python3
import socket
import sys

def getTCSport():
    """
        Get the TCS port from the arguments  and return them, if no arguments are given, return default port value
    """
    try: # [./TCS, -p, TCSport]
        if sys.argv[1] == "-p" and isinstance(eval(sys.argv[2]), int):
            return int(sys.argv[2])
    except (IndexError, NameError, SyntaxError):
        return 58044
    return 58044 # default port value is  58000+GN (Group Number) -> 58044

def createUDPsocket():
    """
        Create a UDP socket and return it
    """
    try:
        return socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    except socket.error:
        print('Failed to create socket')
        sys.exit()

def bindUDPsocket(udpSocket, host, port):
    """
        Bind the UDP socket to local host and port
    """
    try:
        udpSocket.bind((host, port))
    except socket.error as msg:
        print('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
        sys.exit()

def sendUDPmessage(message, clientSocket, host, port):
    """
        Send a message to the client, using the UDP socket, to the given address (host, port)
        Receive the message to send, the socket that it will be used  and the host port destination
    """
    try:
        clientSocket.sendto(message.encode(), (host, port))
    except socket.error as msg:
        print('Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
        sys.exit()

def receiveUDPmessage(clientSocket):
    """
        Receive a message from the client, using the UDP socket, return the message receveid and the address that it came from
    """
    try:
        data, addr = clientSocket.recvfrom(1024)
        return [data.decode(), addr]
    except socket.error as msg:
        print('Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
        sys.exit()

def isDuplicateLanguage(language, ip, port):
    """
        Open the languages file, and check if an language, ip and port, are already in that file
    """
    currentLanguages = open("languages.txt", mode="r").readlines()
    new_language = " ".join([language,str(ip),str(port)])
    for item in currentLanguages:
        if new_language in item:
            return True
    return False

def getIndexOfLanguage(language, ip, port):
    """
        Get the file's line number of the given language        
    """
    currentLanguages = open("languages.txt", mode="r").readlines()
    new_language = " ".join([language,str(ip),str(port)]) + "\n"
    return currentLanguages.index(new_language)


def main():
    host = ''
    port = getTCSport()
    print("TCS port: " + str(port))
    s = createUDPsocket()
    bindUDPsocket(s, host, port)
    try:
        while(1):
            command, addr = receiveUDPmessage(s) #receive data from client (data, addr)
            if "ULQ" in command: #List from the user client
                print(command)
                if command != "ULQ\n":
                    sendUDPmessage("ULR ERR\n", s, addr[0], addr[1])
                    continue
                try:
                    languages = open("languages.txt", mode="r").readlines() #Open the languages file
                except FileNotFoundError:
                    sendUDPmessage("ULR EOF\n", s, addr[0], addr[1])
                    continue
                languagesString = ""
                for line in languages: #Get all the languages that are available
                    if line != "\n":
                        languagesString+= " " + line.split()[0]
                if len(languages) == 0:
                    sendUDPmessage("ULR EOF\n", s, addr[0], addr[1]) #Respond to the user client
                else:
                    sendUDPmessage("ULR "+ str(len(languages)) + languagesString + "\n", s, addr[0], addr[1]) #Respond to the user client
            elif "UNQ" in command: #Request command from the user client
                print(command)
                if command[:4] != "UNQ " or command[-1] != "\n":
                    sendUDPmessage("UNR ERR\n", s, addr[0], addr[1])
                    continue
                try:
                    command, languageRequest = command.split()
                except ValueError as error:
                    print(error)
                    sendUDPmessage("UNR ERR\n", s, addr[0], addr[1])
                    continue
                languagesFile = open("languages.txt", mode="r").readlines()
                if not isinstance(languageRequest, str):
                    sendUDPmessage("UNR ERR\n", s, addr[0], addr[1])
                    continue
                languageExists = False
                for line in languagesFile:
                    if languageRequest in line:
                        languageName ,languageTRSip, languageTRSport = line.split()
                        message = "UNR " + languageTRSip + " " + languageTRSport + "\n"
                        sendUDPmessage(message, s, addr[0], addr[1])
                        languageExists = True
                        continue
                if not languageExists:
                    message = "UNR EOF\n"
                    sendUDPmessage(message, s, addr[0], addr[1])
                    continue

            elif "SRG" in command:
                # command to register a TRS language in the TCS files
                print(command)
                if command[:4] != "SRG " or command[-1] != "\n":
                    sendUDPmessage("SRR ERR\n", s, addr[0], addr[1])
                    continue
                try:
                    tokenSRG, language, ipTRS, portTRS = command.split()
                except ValueError as error:
                    print(error)
                    sendUDPmessage("SRR ERR\n", s, addr[0], addr[1])
                    continue
                try:
                    if isDuplicateLanguage(language, ipTRS, portTRS):
                        sendUDPmessage("SRR NOK\n", s, addr[0], addr[1])
                    else:
                        open("languages.txt", mode="a").write(" ".join([language,str(ipTRS), str(portTRS)]) + "\n")
                        sendUDPmessage("SRR OK\n", s, addr[0], addr[1])
                except FileNotFoundError as error:
                    open("languages.txt", mode="w").write(" ".join([language,str(ipTRS), str(portTRS)]) + "\n")
                    sendUDPmessage("SRR OK\n", s, addr[0], addr[1])

            elif "SUN" in command:
                # command to unregister a TRS language from the TCS files
                print(command)
                if command[:4] != "SUN " or command[-1] != "\n":
                    sendUDPmessage("SRR ERR\n", s, addr[0], addr[1])
                    continue
                try:
                    tokenSUN, language, ipTRS, portTRS = command.split()
                except ValueError as error:
                    sendUDPmessage("SUR ERR\n", s, addr[0], addr[1])
                    continue
                try:
                    index = getIndexOfLanguage(language, ipTRS, portTRS)
                    oldData = open("languages.txt", mode="r").readlines()
                    oldData.pop(index)
                    open("languages.txt", mode="w").write("".join(oldData))
                    sendUDPmessage("SUR OK\n", s, addr[0], addr[1])
                except ValueError as error:
                    print(error)
                    sendUDPmessage("SUR NOK\n", s, addr[0], addr[1])
                    continue

            else:
                print("Received the following command: " + command)
    except KeyboardInterrupt:
        print("Terminar graciosamente TCS...")
        sys.exit()

if __name__ == '__main__':
    main()
