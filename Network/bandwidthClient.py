import sys
import time
import socket

address = ('127.0.0.1', 33334)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
s.connect(address)
maxspeed = 0
size =  1 * 1024 * 1024

for i in range(100):
    s.send('handshake'.encode('utf-8'))
    recvlen = 0
    totaltime = 0

    start = time.time()
    while recvlen<size:
        recvdata = s.recv(size)
        recvlen += sys.getsizeof(recvdata)
    end = time.time()
    totaltime = end - start
    maxspeed = max(maxspeed,size/totaltime)
print(maxspeed/1024/1024)
s.close()