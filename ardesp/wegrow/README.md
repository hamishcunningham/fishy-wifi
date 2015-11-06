# WeGrow...

...cloud backend / dashboard / thingy source tree


## Running CouchDB

- Install [docker][docker]
- Install [docker-compose][docker-compose] (e.g. "sudo pip install -U docker-compose")
- Install functools32 (e.g. "sudo pip install -U functools32")
- In this directory, run: "DOCKER_HOST=localhost sudo docker-compose up"

## Installing fishyapp on CouchDB

- Install [couchapp][couchapp]:
  "sudo pip install couchapp" (make sure you're using python 2.7 e.g. "python -V)
- cd fishyapp
- couchapp push fishydata (will start app, reports url to load, when done press Control C to stop)

[docker]: http://docs.docker.com/engine/installation/
[docker-compose]: https://docs.docker.com/compose/install/
[couchapp]: https://couchapp.readthedocs.org/en/latest/couchapp/install.html
