#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

# include <kclient.h>

struct tests_device {
    struct kclient *kcl;        // KClient
    dev_id_t id;                // Device ID
    op_id_t send_std_array_ref; // "SEND_STD_ARRAY" reference
    op_id_t set_buffer_ref;     // "SET_BUFFER" reference
    op_id_t read_int_ref;       // "READ_INT" reference
    op_id_t read_uint_ref;      // "READ_UINT" reference
};

void tests_init(struct tests_device *dev, struct kclient *kcl)
{
    dev->kcl = kcl;
    dev->id = get_device_id(kcl, "TESTS");
    dev->send_std_array_ref = get_op_id(kcl, dev->id, "SEND_STD_ARRAY");
    dev->set_buffer_ref = get_op_id(kcl, dev->id, "SET_BUFFER");
    dev->read_int_ref = get_op_id(kcl, dev->id, "READ_INT");
    dev->read_uint_ref = get_op_id(kcl, dev->id, "READ_UINT");

    assert(   dev->send_std_array_ref >= 0 
           && dev->set_buffer_ref     >= 0
           && dev->read_int_ref       >= 0
           && dev->read_uint_ref      >= 0);
}

int tests_get_std_array(struct tests_device *dev)
{
    int i;
    float *buff;

    if (kclient_send_command(dev->kcl, dev->id, dev->send_std_array_ref, "") < 0)
        return -1;

    if (kclient_rcv_array(dev->kcl, 10, float) < 0) {
        fprintf(stderr, "Cannot read data\n");
        return -1;
    }

    buff = kclient_get_buffer(dev->kcl, float);
    for (i=0; i<kclient_get_len(dev->kcl, float); i++)
        if (buff[i] != i*i)
            return -1;

    return 0;
}

#define BUFF_LEN 10

int tests_set_buffer(struct tests_device *dev)
{
    int i;
    uint32_t data[BUFF_LEN];
    bool is_ok;

    for (i=0; i<BUFF_LEN; i++)
        data[i] = i*i;

    if (kclient_send_command(dev->kcl, dev->id, dev->set_buffer_ref, "u", BUFF_LEN) < 0)
        return -1;

    if (kclient_send_array(dev->kcl, data, BUFF_LEN) < 0)
        return -1;

    if (kclient_read_bool(dev->kcl, &is_ok))
        return -1;

    return is_ok ? 0 : -1;
}

int tests_read_int(struct tests_device *dev)
{
    int rcv_int;

    if (kclient_send_command(dev->kcl, dev->id, dev->read_int_ref, "") < 0)
        return -1;

    if (kclient_read_int(dev->kcl, &rcv_int))
        return -1;

    if (rcv_int != -42)
        return -1;

    return 0;
}

int tests_read_uint(struct tests_device *dev)
{
    uint32_t rcv_uint;

    if (kclient_send_command(dev->kcl, dev->id, dev->read_uint_ref, "") < 0)
        return -1;

    if (kclient_read_u32(dev->kcl, &rcv_uint))
        return -1;

    if (rcv_uint != 42)
        return -1;

    return 0;
}

// http://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"

#define SETUP                                                       \
    struct tests_device dev;                                        \
    struct kclient *kcl = kclient_connect("127.0.0.1", 36000);      \
                                                                    \
    if (kcl == NULL) {                                              \
        fprintf(stderr, "Can't connect to server\n");               \
        exit(EXIT_FAILURE);                                         \
    }                                                               \
                                                                    \
    tests_init(&dev, kcl);                                          \

#define TEARDOWN                                                    \
    kclient_shutdown(kcl);

#define TEST(NAME)                                                  \
do {                                                                \
    SETUP                                                           \
    if (tests_ ##NAME(&dev) == 0)                                   \
        printf(GRN "\xE2\x9C\x93" RESET " %s\n", #NAME);            \
    else {                                                          \
        printf(RED "\xE2\x98\x93" RESET " %s\n", #NAME);            \
        tests_fail++;                                               \
    }                                                               \
    tests_num++;                                                    \
    TEARDOWN                                                        \
} while(0);

int main(void)
{
    int tests_num  = 0;
    int tests_fail = 0;

    TEST(get_std_array)
    TEST(set_buffer)
    TEST(read_int)
    TEST(read_uint)

    if (tests_fail == 0)
        printf(GRN "OK" RESET " -- %u tests passed\n", tests_num);
    else {
        printf(RED "FAIL" RESET " -- %u tests passed - %u failed\n", tests_num - tests_fail, tests_fail);
        exit(EXIT_FAILURE);
    }

    return 0;
}