#ifndef ck_udp_sender_test_h
#define ck_udp_sender_test_h

#include "framework.h"

#ifdef IS_WINDOWS
#include <winsock2.h>
#endif

#include "cppkit/ck_udp_sender.h"

class ck_udp_sender_test : public test_fixture
{
public:
    TEST_SUITE(ck_udp_sender_test);
        TEST(ck_udp_sender_test::TestSend);
        TEST(ck_udp_sender_test::TestAim);
        TEST(ck_udp_sender_test::TestGetSetSendBufferSize);
    TEST_SUITE_END();

    virtual ~ck_udp_sender_test() throw()
    {}

    void setup();
    void teardown();

protected:

    void TestSend();
    void TestAim();
    void TestGetSetSendBufferSize();

private:
    int _val;
    cppkit::ck_string _recvAddress;
};

#endif
