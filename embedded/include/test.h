#ifndef HEADER_TEST_WAL
#define HEADER_TEST_WAL

#include "sx1278_lora.h"
#include "button.h"

int test_recv_setup();
int test_recv_loop();
int test_send_setup();
int test_send_loop(ButtonState button);

// Misc
void test_change_mode(ButtonState button);

#endif //HEADER_TEST_WAL