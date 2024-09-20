#include "packets.h"


// Função para serializar um pacote CONNECT (client -> broker)
int serialize_connect(packet_type_code_t packet_type, char *buffer, size_t buffer_size, const char *client_id, uint16_t keep_alive, uint8_t version) {
    if (buffer == NULL || client_id == NULL) {
        return -1; // Erro: parâmetros inválidos
    }

    // Calcular o comprimento dos campos variáveis
    size_t client_id_len = strlen(client_id);
    size_t protocol_name_len = 4; // "MQTT"
    size_t variable_header_len = 2 + protocol_name_len + 1 + 1 + 2; // Tamanho base

    // Adicionar espaço para novos campos em MQTT 5.0, se necessário *NAO-FUNCIONAL-->SOMENTE A IDEIA*
    if (version == 0x05) {
        // Por exemplo, você poderia adicionar espaço para Session Expiry Interval
        variable_header_len += 2; // Exemplo: adicionar 2 bytes
    }

    size_t payload_len = 2 + client_id_len; // 2 bytes para client_id length + client_id
    size_t remaining_length = variable_header_len + payload_len;

    // Verificar se o buffer tem espaço suficiente
    if (2 + remaining_length > buffer_size) {
        return -2; // Erro: tamanho do buffer insuficiente
    }

    // Fixed Header
    buffer[0] = (packet_type << 4); // Tipo de pacote no alto de 4 bits
    buffer[1] = remaining_length;    // Comprimento restante

    // Variable Header
    size_t index = 2;

    // Protocol Name Length (2 bytes)
    buffer[index++] = 0x00;
    buffer[index++] = protocol_name_len;

    // Protocol Name ("MQTT")
    memcpy(buffer + index, "MQTT", protocol_name_len);
    index += protocol_name_len;

    // Protocol Level
    buffer[index++] = version;

    // Connect Flags
    uint8_t connect_flags = 0x00; // Ajuste conforme a necessidade
    if (version == 0x05) {
        connect_flags |= 0x01; // Exemplo: setar um flag para MQTT 5.0
    }
    buffer[index++] = connect_flags;

    // Keep Alive (2 bytes)
    buffer[index++] = (keep_alive >> 8) & 0xFF; // MSB
    buffer[index++] = keep_alive & 0xFF;        // LSB

    // Payload
    // Client ID Length (2 bytes)
    buffer[index++] = (client_id_len >> 8) & 0xFF; // MSB
    buffer[index++] = client_id_len & 0xFF;        // LSB

    // Client ID
    memcpy(buffer + index, client_id, client_id_len);
    index += client_id_len;

    // Retorna o tamanho total do pacote serializado
    return index;
}


int serialize_connack(packet_type_code_t packet_type, char *buffer, size_t buffer_size, uint8_t session_present, uint8_t return_code) {
    if (buffer == NULL || buffer_size < 2) {
        return -1;  // Buffer inválido ou tamanho insuficiente
    }

    // Verifica se o return_code é válido
    if (return_code > 0x05) {
        return -2;  // Código de retorno inválido
    }

    // Preenche o buffer com o tipo de pacote (CONNACK é 0x20)
    buffer[0] = packet_type;  // Aqui, packet_type deve ser 0x20 para CONNACK

    // O segundo byte é o campo "Flags" (session_present bit) e o código de retorno
    buffer[1] = (session_present << 1) | (return_code & 0x0F);

    return 2; 
}



int encode_remaining_length(size_t length, char *buffer, size_t buffer_size) {
    size_t index = 0;
    do {
        unsigned char encoded_byte = length & 0x7F; // 7 bits
        length >>= 7; // Move para o próximo byte
        if (length > 0) {
            encoded_byte |= 0x80; // Define o bit mais significativo para indicar mais bytes
        }
        if (index >= buffer_size) {
            return -1; // Erro: buffer insuficiente
        }
        buffer[index++] = encoded_byte;
    } while (length > 0);

    return index; // Retorna o número de bytes usados
}

int serialize_publish(packet_type_code_t packet_type, char *buffer, size_t buffer_size, const char *topic, const char *message, uint16_t message_id, uint8_t qos, uint8_t retain, uint8_t dup) {
    if (buffer == NULL || topic == NULL || message == NULL) {
        return -1; // parametros invalidos
    }

    size_t topic_len = strlen(topic);
    size_t message_len = strlen(message);
    size_t remaining_length = 2 + topic_len + message_len; // 2 bytes para o comprimento do tópico

    if (qos > 0) {
        remaining_length += 2; // 2 bytes para Message ID
    }

    if (1 + remaining_length > buffer_size) {
        return -2; // buffer insuficiente
    }

    size_t index = 0;

    // Fixed Header
    buffer[index++] = (packet_type << 4) | (dup << 3) | (qos & 0x03) | (retain & 0x01);

    // Remaining Length
    int encoded_length = encode_remaining_length(remaining_length, buffer + index, buffer_size - index);
    if (encoded_length < 0) {
        return -3; // Erro na codificação do comprimento
    }
    index += encoded_length;

    // Variable Header
    // Tópico Length (2 bytes)
    buffer[index++] = (topic_len >> 8) & 0xFF; // MSB
    buffer[index++] = topic_len & 0xFF;        // LSB

    // Tópico
    memcpy(buffer + index, topic, topic_len);
    index += topic_len;

    // Message ID (2 bytes) se QoS > 0
    if (qos > 0) {
        buffer[index++] = (message_id >> 8) & 0xFF; // MSB
        buffer[index++] = message_id & 0xFF;        // LSB
    }

    // Payload (Mensagem)
    memcpy(buffer + index, message, message_len);
    index += message_len;

    return index; // Retorna o tamanho total do pacote serializado(pra utilizar no tamanho do buffer do pacote de envio)
}

int serialize_disconnect(packet_type_code_t packet_type, char *buffer, size_t buffer_size){
    if (buffer == NULL) {
        return -1; // Erro: parâmetros inválidos
    }

    size_t index = 0;

    // Fixed Header
    buffer[index++] = packet_type; // Tipo de pacote no alto de 4 bits
    buffer[index++] = 0x00;            // Comprimento restante

    if (index > buffer_size) {
        return -2; // Erro: buffer insuficiente
    }

    return index;

};

int serialize_pingreq(packet_type_code_t packet_type, char *buffer, size_t buffer_size){

    
    if (buffer == NULL) {
        return -1; // Erro: parâmetros inválidos
    }

    size_t index = 0;

    // Fixed Header
    buffer[index++] = packet_type; // Tipo de pacote no alto de 4 bits
    buffer[index++] = 0x00;            // Comprimento restante

    if (index > buffer_size) {
        return -2; // Erro: buffer insuficiente
    }

    return index;
};