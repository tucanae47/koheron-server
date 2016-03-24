#include <stdlib.h>
#include <stdio.h>

# include <kclient.h>

struct tests_device {
    struct kclient *kcl;        // KClient
    dev_id_t id;                // Device ID
    op_id_t send_std_array_ref; // "SEND_STD_ARRAY" reference
    op_id_t set_buffer_ref;     // "SET_BUFFER" reference
    op_id_t read_int_ref;       // "READ_INT" reference
    op_id_t read_uint_ref;      // "READ_UINT" reference
};

struct tests_device* tests_init(struct kclient * kcl)
{
    struct tests_device *ret_dev = malloc(sizeof(*ret_dev));
    
    if (ret_dev == NULL) {
        fprintf(stderr, "Can't allocate tests_device memory\n");
        return NULL;
    }

    struct tests_device dev = {
        .kcl = kcl,
        .id = get_device_id(kcl, "TESTS"),
        .send_std_array_ref = get_op_id(kcl, dev.id, "SEND_STD_ARRAY"),
        .set_buffer_ref = get_op_id(kcl, dev.id, "SET_BUFFER"),
        .read_int_ref = get_op_id(kcl, dev.id, "READ_INT"),
        .read_uint_ref = get_op_id(kcl, dev.id, "READ_UINT")
    };

    memcpy(ret_dev, &dev, sizeof(struct tests_device));
    return ret_dev;
}

int tests_get_std_array(struct tests_device *dev)
{
    int i;
    float *buff;
    struct command cmd;
    init_command(&cmd, dev->id, dev->send_std_array_ref);

    if (kclient_send(dev->kcl, &cmd) < 0)
        return -1;

    if (kclient_rcv_array(dev->kcl, 10, float) < 0) {
        fprintf(stderr, "Cannot read data\n");
        return -1;
    }

    buff = kclient_get_buffer(dev->kcl, float);
    for (i=0; i<kclient_get_len(dev->kcl, float); i++)
        printf("%i => %f\n", i, buff[i]);

    return 0;
}

#define BUFF_LEN 20

int tests_set_buffer(struct tests_device *dev)
{
    int i;
    uint32_t data[BUFF_LEN];
    struct command cmd;

    for (i=0; i<BUFF_LEN; i++)
        data[i] = i*i;

    init_command(&cmd, dev->id, dev->set_buffer_ref);
    if (add_parameter(&cmd, BUFF_LEN) < 0) return -1;

    if (kclient_send(dev->kcl, &cmd) < 0)
        return -1;

    if (kclient_send_array(dev->kcl, data, BUFF_LEN) < 0)
        return -1;

    return 0;
}

int tests_read_int(struct tests_device *dev)
{
    int8_t rcv_int;
    struct command cmd;
    init_command(&cmd, dev->id, dev->read_int_ref);

    if (kclient_send(dev->kcl, &cmd) < 0)
        return -1;

    if (kclient_read_int(dev->kcl, &rcv_int))
        return -1;

    printf("Received int %i\n", rcv_int);
    return 0;
}

int tests_read_uint(struct tests_device *dev)
{
    uint32_t rcv_uint;
    struct command cmd;
    init_command(&cmd, dev->id, dev->read_uint_ref);

    if (kclient_send(dev->kcl, &cmd) < 0)
        return -1;

    if (kclient_read_u32(dev->kcl, &rcv_uint))
        return -1;

    printf("Received int %i\n", rcv_uint);
    return 0;
}

int main(void)
{
    struct kclient *kcl = kclient_connect("127.0.0.1", 36100);

    if (kcl == NULL) {
        fprintf(stderr, "Can't connect to server\n");
        exit(EXIT_FAILURE);
    }

    struct tests_device *dev = tests_init(kcl);
    tests_get_std_array(dev);
    tests_set_buffer(dev);
    tests_read_int(dev);
    tests_read_uint(dev);

    free(dev);
    kclient_shutdown(kcl);
    return 0;
}