
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2013, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#ifndef cppkit_actor_h
#define cppkit_actor_h

#include <mutex>
#include <condition_variable>
#include <future>
#include <list>

namespace cppkit
{

template<class CMD, class RESULT>
class actor
{
    typedef std::unique_lock<std::recursive_mutex> guard;

public:
    actor() :
        _thread(),
        _lock(),
        _cond(),
        _queue(),
        _started( false )
    {
    }

    actor( const actor& ) = delete;

    virtual ~actor() noexcept
    {
        if( started() )
            stop();
    }

    actor& operator = ( const actor& ) = delete;

    void start()
    {
        guard g( _lock );

        _started = true;

        _thread = std::thread( &actor<CMD,RESULT>::_entry_point, this );
    }

    inline bool started()
    {
        return _started;
    }

    void stop()
    {
        if( started() )
        {
            {
                guard g( _lock );

                _started = false;

                _cond.notify_one();

                _queue.clear();
            }

            _thread.join();
        }
    }

    std::future<RESULT> post( const CMD& cmd )
    {
        guard g( _lock );

        std::promise<RESULT> p;
        std::future<RESULT> waiter = p.get_future();

        _queue.push_front( std::pair<CMD,std::promise<RESULT>>( cmd, std::move( p ) ) );

        _cond.notify_one();

        return waiter;
    }

    virtual RESULT process( const CMD& cmd ) = 0;

protected:
    void _entry_point()
    {
        while( _started )
        {
            std::unique_lock<std::recursive_mutex> g( _lock );

            _cond.wait( g, [this](){return !this->_queue.empty() ? true : !this->_started;} );

            if( !_started )
                continue;

            _queue.back().second.set_value( process( _queue.back().first ) );
            _queue.pop_back();
        }
    }

    std::thread _thread;
    std::recursive_mutex _lock;
    std::condition_variable_any _cond;
    std::list<std::pair<CMD,std::promise<RESULT>>> _queue;
    bool _started = false;
};

}

#endif
