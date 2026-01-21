### RFC

Command: JOIN
Parameters: <channel>{,<channel>} [<key>{,<key>}]

1. the user must be invited if the channel is invite-only
2. the correct key (password) must be given if it is set
3. the correct key (password) must be given if it is set

Once a user has joined a channel, they receive notice about all
commands their server receives which affect the channel. This
includes MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.
