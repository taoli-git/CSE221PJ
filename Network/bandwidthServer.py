import sys
import time
import socket

host = '127.0.0.1'
port = 33334
addr = (host,port)

tcpSerSock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
tcpSerSock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
tcpSerSock.bind(addr)
tcpSerSock.setblocking(1)
tcpSerSock.listen(1)

fileSize =  2 * 1024 * 1024
file = '1' * fileSize
msg = file.encode('utf-8')

print("Waiting connect...")
tcpCliSock, addr = tcpSerSock.accept()
tcpCliSock.settimeout(20.0)
print('...connected from', addr)
while 1:
    sign = tcpCliSock.recv(9)
    if not sign: break
    tcpCliSock.sendall(msg)
tcpCliSock.close()
