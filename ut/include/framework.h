
#ifndef cppkit_ut_framework_h
#define cppkit_ut_framework_h

#include <list>
#include <memory>
#include <mutex>
#include <exception>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <stdio.h>

void ut_usleep(unsigned int usec);

#define TEST_SUITE(a) \
        a() : test_fixture(#a) {

#define TEST(a) \
        add_test((void (test_fixture::*)())&a,#a)

#define TEST_SUITE_END() }

class test_fixture;

struct test_container
{
    test_container() :
        fixture(NULL),
        test(NULL),
        test_name(),
        exception_msg(),
        passed(false)
    {
    }

    test_fixture* fixture;
    void (test_fixture::*test)();
    std::string test_name;
    std::string exception_msg;
    bool passed;
};

class ut_fail_exception : public std::exception
{
public:
    ut_fail_exception()
        : std::exception(),
          _line_num( 0 ),
          _src_file(),
          _what_msg(),
          _msg()
    {
    }

    virtual ~ut_fail_exception() noexcept
    {
    }

    virtual const char* what() const throw()
    {
        return _what_msg.c_str();
    }

    const char* get_msg() const
    {
        return _msg.c_str();
    }

    void set_msg(std::string msg)
    {
        _msg = msg;

        char line_num_msg[36];
        snprintf(line_num_msg,36,"%d",_line_num);

        _what_msg = _msg + " in file " + _src_file + " at line " + line_num_msg;
    }

    void set_throw_point(int line, const char* file)
    {
        _line_num = line;
        _src_file = file;

        char line_num_msg[36];
        snprintf(line_num_msg,36,"%d",_line_num);

        _what_msg = _msg + " in file " + _src_file + " at line " + line_num_msg;
    }

    int get_line_num() const
    {
        return _line_num;
    }

    const char* get_src_file() const
    {
        return _src_file.c_str();
    }

private:
    int _line_num;
    std::string _src_file;
    mutable std::string _what_msg;
    std::string _msg;
};

#define UT_ASSERT(a) if( !(a) ) { ut_fail_exception e; e.set_msg(#a); e.set_throw_point(__LINE__,__FILE__); throw e; }
#define UT_ASSERT_THROWS(a,b) try { bool threw = false; try { (a); } catch( b& ex ) { threw=true; } if(!threw) throw false; } catch(...) {ut_fail_exception e; e.set_msg("Test failed to throw expected exception."); e.set_throw_point(__LINE__,__FILE__); throw e;}
#define UT_ASSERT_NO_THROW(a) { bool threw = false; try { (a); } catch( ... ) { threw=true; } if(threw) {ut_fail_exception e; e.set_msg("Test threw unexpected exception."); e.set_throw_point(__LINE__,__FILE__); throw e;} }

extern std::recursive_mutex _test_fixtures_lock;

class test_fixture
{
public:
    test_fixture(std::string fixture_name)
        : _tests(),
          _something_failed( false ),
          _fixture_name( fixture_name )
    {
    }

    virtual ~test_fixture() noexcept
    {
    }

    void run_tests()
    {
        std::lock_guard<std::recursive_mutex> g(_test_fixtures_lock);

        auto i = _tests.begin();
        for( ; i != _tests.end(); i++ )
        {
            setup();

            try
            {
                printf("%-64s [",(*i).test_name.c_str());
                ((*i).fixture->*(*i).test)();
                (*i).passed = true;
                printf("P]\n");
            }
            catch(std::exception& ex)
            {
                _something_failed = true;
                (*i).passed = false;
                (*i).exception_msg = ex.what();
                printf("F]\n");
            }
            catch(...)
            {
                _something_failed = true;
                (*i).passed = false;
                printf("F]\n");
            }

            teardown();
        }
    }

    bool something_failed()
    {
        return _something_failed;
    }

    void print_failures()
    {
        std::lock_guard<std::recursive_mutex> g(_test_fixtures_lock);

        auto i = _tests.begin();
        for( ; i != _tests.end(); i++ )
        {
            if(!(*i).passed)
            {
                printf("\nUT_FAIL: %s failed with exception: %s\n",
                       (*i).test_name.c_str(),
                       (*i).exception_msg.c_str());
            }
        }
    }

    std::string get_name() const { return _fixture_name; }

protected:
    virtual void setup() {}
    virtual void teardown() {}
    void add_test( void (test_fixture::*test)(), std::string name )
    {
        std::lock_guard<std::recursive_mutex> g(_test_fixtures_lock);
        struct test_container tc;
        tc.test = test;
        tc.test_name = name;
        tc.fixture = this;
        _tests.push_back( tc );
    }

    std::list<struct test_container> _tests;
    bool _something_failed;
    std::string _fixture_name;
};

extern std::list<std::shared_ptr<test_fixture> > _test_fixtures;

#define REGISTER_TEST_FIXTURE(a) \
class a##_static_init \
{ \
public: \
    a##_static_init() { \
        _test_fixtures.push_back(make_shared<a>()); \
    } \
}; \
a##_static_init a##_static_init_instance;

// This is a globally (across test) incrementing counter so that tests can avoid having hardcoded port
// numbers but can avoid stepping on eachothers ports.
extern int _next_port;

int ut_next_port();

#define UT_NEXT_PORT() ut_next_port()

#endif