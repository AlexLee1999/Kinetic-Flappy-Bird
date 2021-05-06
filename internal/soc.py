import socket
import json
import sys

HOST = sys.argv[1]
PORT = 65432

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST,PORT))
    print(f'Bind {HOST}:{PORT}')
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f'Connected by {addr}')
        while True:
            data = conn.recv(1024).decode('utf-8')
            print(f"Recieved from socket:  {data}")
            j_data = json.loads(data)
            if j_data['z'] > 1500 or j_data['z'] < 300:
                print("Detected")
