#include "../../includes/client/client.hpp"

void Client::clearOutBuffer(size_t bytes) {
    if (bytes >= _outBuffer.length())
        _outBuffer.clear();
    else
        _outBuffer.erase(0, bytes);
}