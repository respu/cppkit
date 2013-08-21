
CXXFLAGS=-std=c++11 -I include -D LINUX_OS -fPIC -g
OFILES=source/ck_string.o source/ck_logger.o source/ck_stack_trace.o source/ck_error_msg.o source/ck_socket_address.o source/ck_time_utils.o source/ck_socket.o source/ck_sprintf.o source/ck_exception.o source/ck_memory.o source/ck_byte_ptr.o source/ck_large_files.o

libcppkit.so : $(OFILES)
	@echo linking
	g++ -g -shared -o libcppkit.so -rdynamic -fPIC $(OFILES) -l pthread -l dl

clean :
	@echo cleaning
	@rm -f $(OFILES)
	@rm -f libcppkit.so
	@rm -f *~
	@rm -f include/cppkit/*~
	@rm -f include/cppkit/os/*~
	@rm -f source/*~
