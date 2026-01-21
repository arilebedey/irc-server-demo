#include "../../includes/command/command.hpp"

void Command::welcomeUser(Client *caller)
{
    if (!caller->getIsRegistered()) {
        std::cout << "User not registered yet" << std::endl;
        return;
    }
    if (caller->getWelcome())
    {
        std::cout << "User already welcomed" << std::endl;
        return;
    }
    _server->sendMessage(caller, infoSuccesConnexion());
    caller->setWelcome(true);   
}