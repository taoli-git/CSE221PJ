import sys
import time
import socket

host = '127.0.0.1'
port = 33333
addr = (host,port)

tcpSerSock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
tcpSerSock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
tcpSerSock.bind(addr)
tcpSerSock.setblocking(1)
tcpSerSock.listen(1)


while 1:
    print("Waiting connect...")
    tcpCliSock, addr = tcpSerSock.accept()
    print('...connected from', addr)

    while 1:
        recvdata = tcpCliSock.recv(64)
        if not recvdata: break
        tcpCliSock.sendall(recvdata)

    tcpCliSock.close()
