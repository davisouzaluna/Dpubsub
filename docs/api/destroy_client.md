Destroy a client. It receives a client_t pointer. Close the socket, free IP address and client ID. In summary, free resources.

# Syntax

```c
int destroy_client(client_t *client);
```

# Parameters

`client_t *client`

A client struct

# Return

Return **0** if Sucess and **-1** if failed.

## header: `client.h`