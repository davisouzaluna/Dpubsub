# Receive message function

Receive a message from the broker/server. The protocol is defined in enum `protocol_t`.

# Syntax

```c
int receive_message(client_t *client, protocol_t protocol);
```

# Parameters

`client_t *client`

A client struct 

`protocol_t protocol`

Enum to define the protocol used to connect to the broker

# Return
Return **0** if Sucess and **-1** if failed.

## header: `client.h`