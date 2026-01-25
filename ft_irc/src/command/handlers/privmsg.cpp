#include "../../../includes/command/command.hpp"

void Command::privmsg() {
  if (_args.empty() || _args.size() != 1 || _trailing.empty())
    return;
  std::string target = _args[0];
  if (target.c_str()[0] == '#' || target.c_str()[0] == '&') {
    // the target is a channel.
    // TODO : check if the user is able to chat in this channel or not.
    target = target.erase(0, 1);
    Channel *channel = _server->getChannel(target);
    if (!channel)
      return;
    _server->msgToChannel(channel, _caller,
                         craftMessage(_caller, _args[0], _trailing));
  } else {
    // the target of the message is an other user.
    Client *client = _server->getClientFromNick(target);
    if (!client)
      return;
    _server->sendMessage(client, craftMessage(_caller, _args[0], _trailing));
  }
}
