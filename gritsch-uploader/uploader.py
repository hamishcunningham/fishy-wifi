# For NodeMcu version 0.9.4 build 2014-12-30 and newer.

save_lua = \
r"""file.remove('%s')
    file.open('%s', 'w')
    uart.on('data', '\r',
      function (d)
        d = string.gsub(d, '\r', '')
        if d == '~~END~~' then
          uart.on('data')
          file.close()
        else
          file.write(d)
          uart.write(0, '\r')
        end
      end, 0)"""
save_lua = ' '.join([line.strip().replace(', ', ',') for line in save_lua.split('\n')])

com_port = 'COM19'
baud_rate = 115200
# baud_rate = 9600

import os, serial, sys

ser = serial.Serial(com_port, baud_rate, timeout=1)

def read_till_prompt():
    data = ''
    while data[-3:] != '\n> ':
        data += ser.read()
    return data

line_number = 0
def get_line_number():
    global line_number
    line_number += 1
    return str(line_number).rjust(3) + ':'

def log_lines(data):
    for char in data:
        sys.stdout.write(char)
        if char == '\n' :
            sys.stdout.write(get_line_number())

file_path = sys.argv[1]
file_name = os.path.basename(file_path)
save_command = save_lua % (file_name, file_name) + '\r'
ser.write(save_command)
read_till_prompt()
f = open( file_path, 'rt' ); content = f.read(); f.close()
pos = 0
sys.stdout.write(get_line_number())
chunk_size = 255
while pos < len(content):
    data = content[pos : pos+chunk_size]
    log_lines(data)
    ser.write(data + '\r')
    ser.read(1) # \r
    pos += chunk_size
print
ser.write('~~END~~\r')

ser.write('=1\r')  # Write some command, otherwise the interpreter
read_till_prompt() # does not really return to interactive mode.

# ser.write('dofile("%s")\r' % file_name)
# print read_till_prompt()

ser.close()
