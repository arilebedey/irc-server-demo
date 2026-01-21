#include "../../includes/channel/channel.hpp"
#include "../../includes/server/server.hpp"

bool Server::checkChannelExists(const std::string &name) {
  ChannelMap::iterator it = _channels.find(name);
  if (it != _channels.end())
    return true;
  return false;
}

Channel *Server::getOrCreateChannel(const std::string &name) {
  ChannelMap::iterator it = _channels.find(name);
  if (it != _channels.end())
    return &(it->second);
  _channels[name] = Channel(name);
  return &(_channels[name]);
}

void Server::deleteChannelIfEmpty(const std::string &name) {
  ChannelMap::iterator it = _channels.find(name);
  if (it != _channels.end() && it->second.getMemberCount() == 0)
    _channels.erase(name);
}
