import random
import string
import socket

def ranstr(num):
    H = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'
    salt = ''
    for i in range(num):
        salt += random.choice(H)
    return salt

host = '127.0.0.1'
port = 33334
addr = (host,port)

tcpSerSock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
tcpSerSock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
tcpSerSock.bind(addr)
tcpSerSock.setblocking(1)
tcpSerSock.listen(1)

size =  1 * 1024 * 1024
file = ranstr(size)
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
