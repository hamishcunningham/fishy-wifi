#!/bin/bash

(date; echo) >leak-test-data.txt; ./rs485-api.sh -c run_leak_test |tee -a leak-test-data.txt 2>&1
