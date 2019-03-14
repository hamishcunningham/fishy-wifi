#!/bin/bash

F=~/leak-test-data-`date +%Y-%m-%d`.txt
(date; echo) >${F}
(./rs485-api.sh -c run_solenoid_test; echo; date) |tee -a ${F} 2>&1
