
CXXFLAGS=-std=c++11 -I include -I ../include -D LINUX_OS -fPIC -g
OFILES=source/framework.o source/ck_socket_test.o source/ck_socket_address_test.o source/ck_string_test.o source/ck_byte_ptr_test.o source/ck_memory_test.o source/recursive_mutex_test.o source/chrono_test.o source/condition_variable_test.o source/ck_large_files_test.o

ut : $(OFILES)
	@echo linking
	g++ -std=c++11 -g -o ut -rdynamic -fPIC $(OFILES) -pthread -L .. -l cppkit

clean :
	@echo cleaning
	@rm -f $(OFILES)
	@rm -f ut
	@rm -f *~
	@rm -f include/*~
	@rm -f source/*~
