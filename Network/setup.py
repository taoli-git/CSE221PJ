import sys
import time
import socket

address = ('127.0.0.1', 33333)
totaltime = 0
for i in range(1000):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    start = time.time()
    s.connect(address)
    end = time.time()
    totaltime += end - start
    s.close()
print(totaltime/1000)

