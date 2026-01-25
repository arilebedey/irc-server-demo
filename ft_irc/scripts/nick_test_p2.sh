#!/bin/bash

HOST="localhost"
PORT="6667"

{
  echo -ne "CAP LS\r\n"
  echo -ne "PASS password\r\n"
  echo -ne "NICK ari\r\n"
  echo -ne "USER ari minecraft fortnite :Ari\r\n"
  echo -ne "CAP END\r\n"
  echo -ne "JOIN #general\r\n"
  sleep 100
} | nc "$HOST" "$PORT"
