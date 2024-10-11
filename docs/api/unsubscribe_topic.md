# Unsubscribe function

Function to send unsubscribe packet to broker.

# Syntax

```c
int unsubscribe_topic(client_t *client, char *topic);
```

# Parameters

`client_t *client`

A client struct 

`char *topic`

Topic to unsubscribe

# Return
Return **0** if Sucess and **-1** if failed.

## header: `client.h`