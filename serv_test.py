#!/usr/bin/env python3
import socket

HOST = '0.0.0.0'  # Standard loopback interface address (localhost)
PORT = 80        # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    date =b'test'
    with conn:
        print('Connected by', addr)
        data = conn.recv(1024)
        print(data)
        while True:
            conn.send(date)

