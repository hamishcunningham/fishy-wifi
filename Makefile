README.html: README.mkd
	pandoc -V title:"ESP8266 notes" -S -N --standalone --toc -t html5 README.mkd -o README.html
