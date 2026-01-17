- Why server PollFd is added to \_fds?
  The server socket itself needs to be monitored by poll() so the server knows when clients are trying to connect.

- Why check `if (fd == this->_servSocketFd) {` before acceptClient() in handleRead?
  poll() watches all file descriptors
