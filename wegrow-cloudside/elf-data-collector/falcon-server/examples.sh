curl -i -H "Accept: application/json" -H "Content-Type: application/json" \
  -d '{"timestamp": 1466451188.0, "airTemp": 13, "humidity": 42, "lux": 1492, "pH": 6.9, "waterTemp": 16}' \
  -X GET 'localhost:8000/collect/9999'
