#!/usr/bin/python3
import socket
import sys
import os

def translationText(language, listOfWords):
    """
        Receives a language and a list of words
        Returns a list of translated words
    """
    txt = open(language+".txt", mode="r").readlines()
    translatedWords = []
    for word in listOfWords:
        for line in txt:
            if line.split()[0] == word:
                translatedWords.append(line.split()[1])
    return translatedWords

def translationFile(language, filename):
    """
        Receives a language and a filename
        Returns a list with the translated filename, filesize and bytes of file
    """
    txt = open(language+"File.txt", mode="r").readlines()
    translatedFileName = ""
    for line in txt:
        if line.split()[0] == filename:
            translatedFileName = line.split()[1]
    if translatedFileName == "":
        return [translatedFileName, 0, ""]
    translatedFileData = open(translatedFileName, mode="rb").read()
    print("Translated filename: ", translatedFileName)
    print("Tranlated filesize: ", len(translatedFileData))
    #print("Translated file data: ", translatedFile)
    return [translatedFileName, len(translatedFileData), translatedFileData]

def getTCSargs():
    """
        Parses the command line arguments and returns the TCS server name and port number
    """
    try:
        nameIndex, portIndex = sys.argv.index("-n"), sys.argv.index("-e")
        if abs(nameIndex - portIndex) > 1:
            if isinstance(sys.argv[nameIndex+1],str) and isinstance(sys.argv[portIndex+1], int):
                return [sys.argv[nameIndex+1], int(sys.argv[portIndex+1])]
    except ValueError as error:
        return ["localhost", 58044]
    except IndexError as error:
        return ["localhost", 58044]
    return ["localhost", 58044]

def getTRSLanguage():
    """
        Parses the command line arguments and returns the TRS language
    """
    try:
        return sys.argv[1]
    except IndexError as error:
        print("No language argument\n")
        sys.exit()

def getTRSPort():
    """
        Parses the command line arguments and returns the TRS server port number
    """
    try:
        portIndex = sys.argv.index("-p")
        if isinstance(eval(sys.argv[portIndex+1]), int):
            return int(sys.argv[portIndex+1])
    except ValueError as error:
        return 59000
    except IndexError as error:
        return 59000

def createUDPsocket():
    """ 
        Creates a UDP socket
    """
    try:
        return socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    except socket.error:
        print('Failed to create socket')
        sys.exit()

def bindUDPsocket(udpSocket, host, port):
    """
        Binds udpSocket to a host and port
    """
    try:
        udpSocket.bind((host, port))
        print('Socket bind complete')
    except socket.error as msg:
        print('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
        sys.exit()

def sendUDPmessage(message, clientSocket, host, port):
    """
        Sends a message to UDP client socket at address given by host and port
    """
    try:
        clientSocket.sendto(message.encode(), (host, port))
    except socket.error as msg:
        print('Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
        sys.exit()

def receiveUDPmessage(clientSocket):
    """
        Receives a UDP message from the clientSocket
    """
    clientSocket.settimeout(5)
    try:
        data, addr = clientSocket.recvfrom(1024)
        return [data.decode(), addr]
    except socket.timeout:
        print("TRS UDP socket timed out. Check if TCS server is active")
        sys.exit()
    except socket.error as msg:
        print('Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
        sys.exit()

def getTRRFileSize(dataBlock):
    """
        Parses the starting packet received of an expected TRQ request
        Returns the size of the file data sent in the TRQ request
    """
    print(dataBlock[0:70])
    if dataBlock[0:4].decode() == "TRQ ":
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

def processFirstBlock(block):
    """
        Checks if the first packet received follows the TRQ protocol
    """
    print(block)
    if block[0:6].decode() == "TRQ t ":
        return ["TRQ", "t"]
    elif block[0:6].decode() == "TRQ f ":
        return ["TRQ", "f"]
    else:
        return ["", ""]

def processTextTranslationRequest(block):
    """
        Checks if the TRQ request has the correct arguments: numberOfWords and listOfWords
    """
    try:
        numberOfWords = int(block[6:7].decode())
        listOfWords = block[8:].decode().split()
        return [numberOfWords, listOfWords]
    except ValueError:
        return [0,""]
    except IndexError:
        return [0,""]

def processFileTranslationRequest(block):
    """
        Receives the starting TRQ packet of type f
        Return a list with the input filename, filesize and filedata
    """
    #[filename, filesize, filedata]
    try:
        filename = ""
        for nameIndex in range(6,len(block)):
            if block[nameIndex:nameIndex+1].decode() == " ":
                break
            filename += block[nameIndex:nameIndex+1].decode()
        print("filename: ", filename)
        filesize = ""
        for sizeIndex in range(nameIndex+1, len(block)):
            if block[sizeIndex:sizeIndex+1].decode() == " ":
                break
            filesize += block[sizeIndex:sizeIndex+1].decode()
        print("filesize: ", filesize)

        filedata = block[sizeIndex+1:]
        print("filedata: ", filedata)
        return [filename, int(filesize), filedata]
    except IndexError:
        return ["",0,""]

def main():
    # check for TRS language
    language = getTRSLanguage()
    if language == "":
        print("No language argument given")
        sys.exit()
    elif language not in ["english", "french"]:
        print("Language not given")
        sys.exit()

    # get TRS and TCS servers info
    portTRS = getTRSPort()
    hostTCS, portTCS = getTCSargs()
    ipTRS = socket.gethostbyname(socket.gethostname())
    print("TRS language: " + language)
    print("TRS IP: " + str(ipTRS))
    print("TRS port: " + str(portTRS))
    print("TCS name: " + str(hostTCS))
    print("TCS port: " + str(portTCS))
    
    # create UDP and TCP sockets
    udpSocket = createUDPsocket()
    tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    tcpSocket.bind(('',portTRS))
    tcpSocket.listen(1)

    # register TCS in the TCS server
    registerTRSmessage = " ".join(["SRG", language, str(ipTRS), str(portTRS)]) + "\n"
    sendUDPmessage(registerTRSmessage, udpSocket, hostTCS, portTCS)
    reply, addr = receiveUDPmessage(udpSocket)

    print(reply)
    if "ERR" in reply:
        print("Protocol syntax error: check if you are sending language, TRS IP, TRS TCP Port")
        sys.exit()
    if"NOK" in reply:
        print("Language already exists in the TCS server")
        sys.exit()

    try:
        while 1:
            # receive first packet from TCP connection
            connectionSocket, addr = tcpSocket.accept()
            firstBlock = connectionSocket.recv(100)
            command, translationType = processFirstBlock(firstBlock)
            if "TRQ" == command:
                # translation request
                if translationType == "t":
                    print("Translating words...")
                    # get number of words and list of words from TRQ request
                    numberOfWords, listOfWords = processTextTranslationRequest(firstBlock)
                    if numberOfWords == 0:
                        connectionSocket.send("TRR NTA\n".encode())
                        continue
                    # get translation for words
                    translation = translationText(language, listOfWords)
                    if len(translation) == numberOfWords:
                        # send translated words to user
                        message = "TRR t " + str(numberOfWords) + " " + " ".join(translation) + "\n"
                        connectionSocket.send(message.encode())
                    else:
                        # at least word couldn't be translated
                        connectionSocket.send("TRR NTA\n".encode())

                elif translationType == "f":
                    print("Translating file...")
                    # process file translation
                    filename, filesize, filedata = processFileTranslationRequest(firstBlock)
                    translatedName, translatedSize, translatedData = translationFile(language, filename)
                    if translatedName == "":
                        # TRS couldn't find a translation match to input filename
                        connectionSocket.send("TRR NTA\n".encode())
                    else:
                        # TRS found a file translation match and will save the input file of user
                        sizeCounter = filesize - len(filedata) - 1
                        while sizeCounter > 0:
                            data_aux = connectionSocket.recv(256)
                            print("received ",len(data_aux)," bytes")
                            filedata += data_aux
                            sizeCounter -= len(data_aux)
                        open(filename, mode="wb").write(filedata)

                        # send the translated file to the user
                        message = "TRR f " + translatedName + " " + str(translatedSize) + " "
                        print(message)
                        connectionSocket.sendall(message.encode()+translatedData)
                else:
                    print("TRQ command wrong syntax")
                    connectionSocket.send("TRR ERR\n".encode())

    except KeyboardInterrupt:
        # catch the ctrl+c signal to implement a smooth exit from the program execution
        print("Terminar graciosamente TRS...")
        # unregister the TRS server from the TCS server
        unregisterTRSmessage = " ".join(["SUN", language, ipTRS, str(portTRS)]) + "\n"
        sendUDPmessage(unregisterTRSmessage, udpSocket, hostTCS, portTCS)
        reply, addr = receiveUDPmessage(udpSocket)
        print(reply)
        # close the TCP socket before terminating
        tcpSocket.close()
        sys.exit()

if __name__ == '__main__':
    main()
