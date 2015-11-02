# WeGrow...

...cloud backend / dashboard / thingy source tree


## Running CouchDB

- Install [docker][docker]
- Install [docker-compose][docker-compose]
- In this directory, run: "docker-compose up"

## Installing fishyapp on CouchDB

- Install [couchapp][couchapp]:
  "sudo pip install couchapp" (make sure you're using python 2.7)
- cd fishyapp
- couchapp push fishydata


[docker]: https://www.docker.com/
[docker-compose]: https://docs.docker.com/compose/install/
[couchapp]: https://couchapp.readthedocs.org/en/latest/couchapp/install.html
