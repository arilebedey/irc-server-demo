# Overview

Minimal IRC server implementation in **C++98** for the 42 School `ft_irc` project.

This project implements a standalone IRC server compatible with a standard IRC client.
It supports multiple simultaneous clients using non-blocking I/O and a single polling
mechanism (`poll()` or equivalent).

## Build

```bash
make
```

## Run

```bash
./ircserv <port> <password>
```

- `port`: TCP port to listen on
- `password`: password required by clients

## Mandatory Features

- Non-blocking TCP server (single `poll()` or equivalent)
- Multiple clients without forking
- Communication over IPv4 or IPv6
- Client registation (IRC protocol specification):
  - `PASS`
  - `NICK`
  - `USER`
- Channel management:
  - `JOIN`, `PART`
  - Channel message broadcasting
- Private messages:
  - `PRIVMSG`
- Regular users
- Channel operators:
  - `KICK` – remove a user from a channel
  - `INVITE` – invite a user to a channel
  - `TOPIC` – set/view channel topic
  - `MODE`:
    - `i` invite-only channel
    - `t` topic restricted to operators
    - `k` channel key (password)
    - `o` give/take operator status
    - `l` user limit

## Important strategies

- Use non-blocking sockets with a single `poll()` loop
- Maintain per-client read and write buffers
- Never send without `poll()` signaling `POLLOUT`
- Validate command order in IRC standard (`PASS` → `NICK` → `USER`)
- Aggregate TCP input per client into buffers until `\r\n` before parsing commands
- Centralize error replies (numeric responses)
- Cleanly remove clients on disconnect or error
- Avoid STL iterator invalidation when modifying client/channel lists (look at correct patterns)
- Enforce channel modes at command entry points
- Treat TCP disconnect (`recv == 0`) as immediate quit

## Basic required tests

- Low bandwidth
- Recieving partial data

## Technical Constraints

- C++98 only
- No external libraries
- No blocking I/O
- One `poll()` (or equivalent) for all FDs
- Compile with `-Wall -Wextra -Werror`

## Notes

- Incoming data is buffered to handle partial reads.
- Commands are processed only after full `\r\n` termination.
- Designed to behave similarly to a real IRC server for supported features.

# Some technical details

## Blocking vs. non-blocking `socket`

Warning! The non-blocking socket below is for demo only and does not use `poll()`, thus violating the rules.

- EAGAIN || EWOULDBLOCK: error code for no data is available to read right now
- O_NONBLOCK: flag that makes a file descriptor non-blocking
- F_SETFL: fcntl command to set file descriptor flags (like O_NONBLOCK)
- AF_INET: address family for IPv4
- SOCK_STREAM: socket type for TCP (connection-oriented, reliable)

```cpp
int fd = socket(AF_INET, SOCK_STREAM, 0);

/* fd is blocking by default */

char buf[512];

recv(fd, buf, sizeof(buf), 0);   // ← execution stops here
printf("This line runs only after data arrives\n");
```

```cpp
int fd = socket(AF_INET, SOCK_STREAM, 0);

/* make socket non-blocking (only allowed flags for fcntl btw) */
fcntl(fd, F_SETFL, O_NONBLOCK);

char buf[512];

/* read() equivalent for network sockets */
ssize_t n = recv(fd, buf, sizeof(buf), 0);

if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    /* no data available right now */
    printf("recv() returned immediately\n");
} else if (n == 0) {
    /* client disconnected */
} else {
    /* data recieved */
}
```

Problems with this :

- spins the CPU
- scales poorly
- breaks under load

## Using `poll()`

```cpp
struct pollfd {
    int   fd;       // file descriptor
    short events;   // what you want to watch
    short revents;  // what actually happened
};
```

```cpp
// fds     : array of pollfd structures (one per file descriptor)
// nfds    : number of elements in the fds array
// timeout : time to wait in milliseconds
//           -1  = wait forever
//            0  = return immediately
//           > 0 = wait up to timeout ms

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

- POLLIN: At least one byte is available to read or the peer closed the connection
- POLLOUT: There is space in the socket send buffer (kernel‑managed memory that temporarily stores data you send before it goes onto the network)
- POLLERR: A socket error occurred (must close the fd)
- POLLHUP: The peer hung up (connection closed)
- POLLNVAL: Invalid file descriptor (bug: fd closed or never valid)

`revents` flags are bitwise :

- POLLIN = 0001
- POLLOUT = 0010
- POLLERR = 0100
- POLLHUP = 1000
- POLLNVAL (set by kernel)

`revents = POLLIN | POLLERR` → 0101

```cpp
struct pollfd pfd;
pfd.fd = fd;
pfd.events = POLLIN;

int ready = poll(&pfd, 1, -1); // kernel waits here

if (ready > 0) {
    if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
        // close connection
    }
    else if (pfd.revents & POLLIN) {
        ssize_t n = recv(fd, buf, sizeof(buf), 0);

        if (n > 0) {
            /* data received */
        } else if (n == 0) {
            /* client disconnected */
        }
    }
}
```
