# WeGrow...

...cloud backend / dashboard / thingy source tree


## Running CouchDB

- Install [docker][docker]
- Install pip (may be already installed) (e.g. "sudo apt-get update && sudo apt-get install python-pip")
- Install [docker-compose][docker-compose] (e.g. "sudo pip install -U docker-compose")
- Install functools32 (may be already installed) (e.g. "sudo pip install -U functools32")
- To start database, in this directory (wegrow), run: "docker-compose up", if you're not set up to run docker
  as a regular user try "DOCKER_HOST=localhost sudo docker-compose up"
- Leave db running to support fishyapp; when all done press Control-C to stop database running

## Installing fishyapp on CouchDB

- Install python-dev (may be already installed) (e.g. "sudo apt-get install python-dev")
- Install [couchapp][couchapp]:
  "sudo pip install couchapp" (make sure you're using python 2.7 e.g. "python -V")
- cd fishyapp
- start app with:
  "couchapp push fishydata"
- the app reports a URL to load e.g. http://127.0.0.1:5984/fishydata/_design/fishyapp/index.html
- open browser and load the URL given to see the data

[docker]: http://docs.docker.com/engine/installation/
[docker-compose]: https://docs.docker.com/compose/install/
[couchapp]: https://couchapp.readthedocs.org/en/latest/couchapp/install.html
