ifndef CXX
CXX := "g++" #use system g++ if not available
endif
ifndef IP
IP := 192.168.1.12 #use system g++ if not available
endif
target := $(shell ${CXX} -dumpmachine)
ifeq (${target}, arm-webos-linux-gnueabi)
$(info Setting compiler include flags for webOS...)
ifndef CXX_FLAGS
CXX_FLAGS := #guess we gotta define this if it no exist
endif
CXX_FLAGS := ${CXX_FLAGS} -I/opt/webos-sdk-x86_64/1.0.g/sysroots/armv7a-neon-webos-linux-gnueabi/usr/include --sysroot=/opt/webos-sdk-x86_64/1.0.g/sysroots/armv7a-neon-webos-linux-gnueabi
endif
make:
	${CXX} -o hbpm -Ilib -lcurl ${CXX_FLAGS} main.cpp
run:
	${CXX} -o hbpm -Ilib -lcurl ${CXX_FLAGS} main.cpp
	./hbpm
deps:
	mkdir -p lib
	cd lib && wget https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp && wget https://github.com/nothings/stb/raw/master/stb_ds.h && wget https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
verbose:
	${CXX} -v -o hbpm -Ilib -lcurl ${CXX_FLAGS} main.cpp
clean:
	@rm hbpm
help:
	@echo 'make' - Build normal version
	@echo 'make run' - Build normal version, and run it
	@echo 'make verbose' - Build normal version with verbose compilation for debugging
	@echo 'make help' - Display this help message
