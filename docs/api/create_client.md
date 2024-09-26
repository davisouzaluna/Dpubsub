# Create_client function

Create a client. It receives a client_t pointer, a client_config_t pointer and a buffer size. If the buffer size is not provided, it will use the default size defined 
in the macro INITIAL_BUFFER_SIZE.It returns 0 if the client is created successfully, -1 otherwise.

# Syntax
```c
int create_client(client_t *client, client_config_t *config, size_t buffer_size);
```

# Parameters

`client_t *client`

A client struct 

`client_config_t *config`

A client config struct

`size_t buffer_size`

Define the buffer size to a client. Default is the macro INITIAL_BUFFER_SIZE.

# Return
Return **0** if Sucess and **-1** if failed.

## header: `client.h`


