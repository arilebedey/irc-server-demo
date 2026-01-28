#!/bin/bash

# Script to test IRC server with netcat

HOST="localhost"
PORT="6667"

# Send commands to the IRC server
{
  echo -ne "CAP LS\r\n"
  echo -ne "PASS password\r\n"
  echo -ne "NICK theaux\r\n"
  echo -ne "USER theaux fortnite fortnite:BABOU Teo\r\n"
  echo -ne "CAP END\r\n"
  echo -ne "JOIN neuille\r\n"
  echo -ne "NICK newman\r\n"
  echo -ne "JOIN #channel\r\n"
  sleep 100
} | nc "$HOST" "$PORT"
