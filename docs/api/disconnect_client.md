# Disconnect client function

Disconnect from the broker. The protocol is defined in enum `protocol_t`.

# Syntax

```c
int disconnect_client(client_t *client, protocol_t protocol);
```
# Parameters


`client_t *client`

A client struct 

`protocol_t protocol`

Enum to define the protocol used to connect to the broker

# Return
Return **0** if Sucess and **-1** if failed.

## header: `client.h`