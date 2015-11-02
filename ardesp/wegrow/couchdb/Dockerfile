FROM ubuntu:14.04

RUN apt-get update && apt-get install -y couchdb
RUN mkdir -p /var/run/couchdb
RUN chown -R couchdb /var/run/couchdb

ADD ./local.ini /etc/couchdb/local.ini
RUN chown -R couchdb /etc/couchdb/

VOLUME /var/lib/couchdb
RUN chown -R couchdb /var/lib/couchdb/

EXPOSE 5984
USER couchdb
CMD ["/usr/bin/couchdb"]
