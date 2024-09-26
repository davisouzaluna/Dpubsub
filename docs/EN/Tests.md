# Performing DpubSub tests

To carry out tests with Dpubsub, it is first important to compile the project. To learn more, see [build](./build).

Right after completing the compilation, enter the tests directory (remembering that you must be inside the build directory). There you will find the executables:

- `api_test`
- `client_test` 

### `api_test`

It will test the functions of `client_tcp_api.h`, connecting, publishing and subscribing to a given topic in the Broker.

### `client_test`

It will test the primary functions of `client.h` and `packets.h`, performing subscription, publishing, sending a ping, connecting and disconnecting with the broker. An experimental function is the manipulation of packets of type *PUBLISH*