- Why server PollFd is added to \_fds?
  The server socket itself needs to be monitored by poll() so the server knows when clients are trying to connect.

-
