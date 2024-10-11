# Publish function

Function to publish a message to broker in a topic.

# Syntax 

```c
int publish(client_t *client, const char *topic, const char *message, uint16_t message_id, uint8_t retain, uint8_t dup);
```

# Parameters


`client_t *client`

A client struct 

`const char *topic`

Topic to publish

`const char *message`

Message to publish

`uint16_t message_id`

Message id. This is important if you want to order your comunication. If you send the message with QoS 0, the message **must be** equal `0`

`uint8_t retain`

A flag to retained the last message. If this flag is enabled, so the broker will retain the last message

`uint8_t dup`

If QoS greater than 0, so this flag indicate retransmission to the message.

# Return
Return **0** if Sucess and **-1** if failed.

## header: `client.h`

