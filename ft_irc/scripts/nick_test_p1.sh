#!/bin/bash

HOST="localhost"
PORT="6667"

{
  echo -ne "CAP LS\r\n"
  echo -ne "PASS password\r\n"
  echo -ne "NICK theaux\r\n"
  echo -ne "USER theaux fortnite fortnite :Teo\r\n"
  echo -ne "CAP END\r\n"
  echo -ne "JOIN #general\r\n"
  sleep 5 
  echo -ne "NICK newman\r\n"
  sleep 2
  echo -ne "PRIVMSG #general :Hello from Teo\r\n"
  sleep 100
} | nc "$HOST" "$PORT"
