import sys
import time
import socket
import random
import string

address = ('127.0.0.1', 33333)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
s.connect(address)
totaltime = 0
str = '1234abcd' * 4
msg = str.encode('utf-8')
for i in range(1000):
    totaltime = 0
    start = time.time()
    s.send(msg)
    data = s.recv(32)
    end = time.time()
    totaltime += end - start
print(totaltime*1000)
s.close()