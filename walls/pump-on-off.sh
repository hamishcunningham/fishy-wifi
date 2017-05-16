#!/bin/bash
# CLI API for the STR2DO14DIN RS-485 controller

FISH_ELF_IP=192.168.1.201

          curl -v "http://${FISH_ELF_IP}/actuate" \
            -H "Origin: http://${FISH_ELF_IP}" \
            -H 'Accept-Encoding: gzip, deflate' \
            -H 'Accept-Language: en-GB,en-US;q=0.8,en;q=0.6' \
            -H 'Upgrade-Insecure-Requests: 1' \
            -H 'User-Agent: curl' \
            -H 'Content-Type: application/x-www-form-urlencoded' \
            -H 'Accept: text/html,application/xhtml+xml,application/xml' \
            -H 'Cache-Control: max-age=0' \
            -H "Referer: http://${FISH_ELF_IP}/" \
            -H 'Connection: keep-alive' \
            --data "state=$1" --compressed
