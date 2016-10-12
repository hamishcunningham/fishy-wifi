import urllib2
import os
import re
import json
import socket


host = "127.0.0.1"
port = 5000
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((host, port))

def parse_line(key, line):
    split = line.split()
    return split[1]

def send_data(dobject):
    data_json = json.dumps(dobject)
    sock.send(data_json)

while True:
    page = urllib2.urlopen("http://192.168.99.1/data")
    lines = page.readlines()
    keys = ["airTemp", "waterTemp", "humidity", "light", "pH", "waterLevel1", "waterLevel2", "waterLevel3"]
    data = {}
    for line in lines:
        if not keys:
            break
        for key in keys:
            if line.find(key) != -1:
                data[key] = parse_line(key, line)    
                keys.remove(key)
        
    print data
    send_data(data)
