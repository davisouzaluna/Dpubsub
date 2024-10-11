# Subscribe function

Client subscribe to a topic. Initially only one topic.

# Syntax

```c
int subscribe_topic(client_t *client, const char *topic, protocol_t protocol);
```

# Parameters

`client_t *client`

A client struct

`const char *topic`

topic to subscribe

`protocol_t protocol`

Enum to define the protocol used to connect to the broker

# Return
Return **0** if Sucess and **-1** if failed.

## header: `client.h`