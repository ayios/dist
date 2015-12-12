PREFIX          = $(realpath ../..)
MACHINE         = `uname -m`
MODULE_DIR 	= ${PREFIX}/modules
MACHINE_DIR     = ${MODULE_DIR}/$(MACHINE)
MODULE_INSTALL_DIR = ${MACHINE_DIR}/std
SLANG_INSTALL_DIR = /usr/local
CC		= gcc
CFLAGS		= -g -O2
SLANG_INC       = -I$(SLANG_INSTALL_DIR)/include
LDFLAGS		= -Wl,-R${SLANG_INSTALL_DIR}/lib
SHARED_LIBS     = -shared -fPIC
PCRE_LIB	= -lpcre
CRYPTO_LIB      = -lssl
INSTALL_MODULE	= install -c -m 644
INSTALL_DIR     = install -v -d -m 755

COMPILE_CMD = $(CC) $(SLANG_INC) $(CFLAGS)
LIBS = $(LDFLAGS) $(SHARED_LIBS) -lm

MODULES = slsmg-module.so termios-module.so select-module.so \
  socket-module.so rand-module.so fork-module.so \
  pcre-module.so getkey-module.so ioctl-module.so \
  crypto-module.so iconv-module.so curl-module.so \
  json-module.so ayios-module.so

all: modules install clean
modules: slsmg-module.so rand-module.so termios-module.so \
         select-module.so pcre-module.so socket-module.so \
         fork-module.so getkey-module.so ioctl-module.so \
         crypto-module.so iconv-module.so curl-module.so \
         json-module.so ayios-module.so
slsmg-module.so:
	$(COMPILE_CMD) slsmg-module.c -o slsmg-module.so $(LIBS)
rand-module.so:
	$(COMPILE_CMD) rand-module.c -o rand-module.so $(LIBS)
termios-module.so:
	$(COMPILE_CMD) termios-module.c -o termios-module.so $(LIBS)
select-module.so:
	$(COMPILE_CMD) select-module.c -o select-module.so $(LIBS)
pcre-module.so:
	$(COMPILE_CMD) pcre-module.c -o pcre-module.so $(PCRE_LIB) $(LIBS)
socket-module.so:
	$(COMPILE_CMD) socket-module.c -o socket-module.so $(LIBS)
fork-module.so:
	$(COMPILE_CMD) fork-module.c -o fork-module.so $(LIBS)
crypto-module.so:
	$(COMPILE_CMD) crypto-module.c -o crypto-module.so $(CRYPTO_LIB) $(LIBS)
getkey-module.so:
	$(COMPILE_CMD) getkey-module.c -o getkey-module.so $(LIBS)
ioctl-module.so:
	$(COMPILE_CMD) ioctl-module.c -o ioctl-module.so $(LIBS)
iconv-module.so:
	$(COMPILE_CMD) iconv-module.c -o iconv-module.so $(LIBS)
curl-module.so:
	$(COMPILE_CMD) curl-module.c -o curl-module.so $(LIBS) -lcurl
json-module.so:
	$(COMPILE_CMD) json-module.c -o json-module.so $(LIBS)
ayios-module.so:
	$(COMPILE_CMD) ayios-module.c -o ayios-module.so $(LIBS) -lpam

install:
	${INSTALL_DIR} ${MODULE_DIR}
	${INSTALL_DIR} ${MACHINE_DIR}
	${INSTALL_DIR} ${MODULE_INSTALL_DIR}
	@for i in $(MODULES); \
	do \
		echo $(INSTALL_MODULE) $$i; \
		$(INSTALL_MODULE) $$i $(MODULE_INSTALL_DIR); \
	done

clean:
	@rm -f socket-module.so fork-module.so getkey-module.so slsmg-module.so\
               rand-module.so pcre-module.so crypto-module.so ioctl-module.so\
               select-module.so socket-module.so termios-module.so iconv-module.so\
	       curl-module.so json-module.so ayios-module.so
