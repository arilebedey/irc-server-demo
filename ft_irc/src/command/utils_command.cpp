#include "../../includes/command/command.hpp"

//: Theaux!theaux@localhost PRIVMSG #42 :Salut tout le monde\r\n

std::string Command::craftMessage(Client *sender, std::string target,
                                  std::string content) {
  std::string message;

  message += ":";
  message += sender->getIdentity();
  message += " PRIVMSG ";
  message += target;
  message += " :";
  message += content;
  message += "\r\n";

  return message;
}

void Command::welcomeUser(Client *caller) {
  if (!caller->getIsRegistered())
    return;
  if (caller->getWelcome())
    return;
  _server->sendMessage(caller, infoSuccesConnexion());
  caller->setWelcome(true);
}