ifndef CXX
CXX := "g++" #use system g++ if not available
endif
ifndef IP
IP := 192.168.1.10
#use my tv's ip if not passed to makefile
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
tv:
	scp hbpm root@${IP}:/media/internal/bin
deps:
	mkdir -p lib
	cd lib && wget https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp && wget https://github.com/nothings/stb/raw/master/stb_ds.h
verbose:
	${CXX} -v -o hbpm -Ilib -lcurl ${CXX_FLAGS} main.cpp
clean:
	@rm hbpm
help:
	@echo 'make' - Build normal version
	@echo 'make run' - Build normal version, and run it
	@echo 'make verbose' - Build normal version with verbose compilation for debugging
	@echo 'make help' - Display this help message
