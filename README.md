# PlainMQ

This is a very simple and lightweight WebSocket message broker written in C++.
It is intended to be fast, *not* reliable, so it should not be used in cases where message
reliability is important. It does not validate message integrity in any way.

## Features
- Basic publish/subscribe flow.
- Password-protected channels (requires password to publish, but not to subscribe).
- Low CPU, memory and bandwidth usage.

## Dependencies
- CMake >= 3.19
- WebSocket++ (https://github.com/zaphoyd/websocketpp)
- Nlohmann JSON library (https://github.com/nlohmann/json)

## Building
```
git clone https://github.com/adamwych/plainmq.git
cd plainmq
mkdir build
cd build
cmake ../
make
```

After those are done, you can start the server using `plainmq` executable.

## Configuration
If you want to change any of the default settings, you can create a JSON file with following structure and pass it as the first argument to `plainmq`:

```js
{
    "host": "127.0.0.1",
    "port": 9999,
    "logsDirectory": "", // Directory in which log files will be stored.
    "logToFileEnabled": true, // Whether logs should be saved to a file.
    "logLevel": "verbose", // default or verbose.
    "reuseAddr": false, // Whether to use REUSEADDR option while making the socket. Behaviour of this option may be different on different operating systems. Use carefully.
    "masterKey": "foo" // Special key required to use the `probe` command.
}
```

All of those properties are optional.

## Commands

### Create
Creates a new channel.

If channel with the same name already exists, then `channel_already_exists` error message will be sent to the client.

If `password` property is specified, then publishing messages on this channel will require client to provide the same password in a `publish` message.

Channels are removed after 600 seconds of inactivity (no new subscriptions or published messages).

```js
{
    "command": "create",
    "name": "channel-name",
    "password": "password" // Optional
}
```

### Subscribe
Starts listening to messages published on a channel.
```js
{
    "command": "subscribe",
    "name": "channel-name"
}
```

### Unsubscribe
Stops listening to messages published on a channel.
```js
{
    "command": "unsubscribe",
    "name": "channel-name"
}
```

### Publish
Publishes a message on a channel.
```js
{
    "command": "publish",
    "name": "channel-name",
    "message": "Hello",
    "password": "password" // Required if channel is password protected.
}
```

### Probe
Returns infomation about the server.
```js
{
    "command": "probe",
    "key": "your master key" // Master key can be specified in the environment settings file.
}
```

## TODO
- Periodic ping to verify connection status
- Remove channels after some time of inactivity