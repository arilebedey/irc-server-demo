### Questions

- Why server PollFd is added to \_fds?
  The server socket itself needs to be monitored by poll() so the server knows when clients are trying to connect.

- Why check `if (fd == this->_servSocketFd) {` before acceptClient() in handleRead?
  poll() watches all file descriptors

- Why \_sigReceived should be `volatile`?
  Compiler might optimize and cache value, thus signal might not change

- Why signalHandler should return `static void`?
  It's a C++ -> C IPC pattern. It ensures signalHandler doesn't require `this` pointer, which C cannot provide. Allows signalHandler to be a C function pointer.

### Potential bugs

- No SIGPIPE handling could cause crashes when send() to disconnected client? Test when broadcasting.

If so, add following to handleSignal():

```cpp
  // ignore signal
  sa.sa_handler = SIG_IGN;
  if (sigaction(SIGPIPE, &sa, NULL) == -1)
    throw(std::runtime_error("sigaction SIGPIPE failed"));
```
