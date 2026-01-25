import socket
import time

sock = socket.socket()
sock.connect(("127.0.0.1", 6667))

commands = [
    "PASS password\r\n",
    "NICK slow\r\n",
    "USER slow 0 * :Slow\r\n",
    "JOIN #c\r\n",
    "PRIVMSG #c :Ok Im VEEEEEEERRRRRRRRRRRYYYYYYYY slow\r\n",
]

for cmd in commands:
    for char in cmd:
        sock.send(char.encode())
        time.sleep(0.1)
    time.sleep(0.5)

sock.close()
