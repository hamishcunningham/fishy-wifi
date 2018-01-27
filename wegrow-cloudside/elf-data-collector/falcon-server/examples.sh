#!/bin/bash

# nc raw request version (working)
cat <<EOF |nc citsci.wegrow.social 8000
GET /collect/elf-12345 HTTP/1.1
User-Agent: curl/7.35.0
Host: localhost:8000
Accept: application/json
Content-Type: application/json
Content-Length: 92

{"timestamp": 1000, "airTemp": 14, "humidity": 44, "lux": 1494, "pH": 6.4, "waterTemp": 14}
EOF
exit $?

# nc file-based raw req version with no trailing newline (working)
cat req.txt | awk 'NR>1{print PREV} {PREV=$0} END{printf("%s",$0)}' | \
  nc localhost 8000
exit $?

# curl parameterised version (working)
curl -i -H "Accept: application/json" -H "Content-Type: application/json" \
  -d '{"timestamp": 999888777, "airTemp": 14, "humidity": 44, "lux": 1494, "pH": 6.4, "waterTemp": 14}' \
  -X GET 'localhost:8000/collect/9999'
exit $?


#to see the actual request, kill gunicorn and:
#
#  nc -l localhost 8000
#
#e.g:
#
#GET /collect/9999 HTTP/1.1
#User-Agent: curl/7.35.0
#Host: localhost:8000
#Accept: application/json
#Content-Type: application/json
#Content-Length: 90
#
#{"timestamp": 200, "airTemp": 14, "humidity": 44, "lux": 1494, "pH": 6.4, "waterTemp": 14}
