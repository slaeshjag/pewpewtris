# Project: pewpewtris
# Makefile configurations

VERSION		=	0.02

PREFIX		=	/usr/local
DATAPATH	=	/usr/share/games/pewpewtris

DBGFLAGS	=	-O0 -g -D__DEBUG__
#DBGFLAGS	=	-O3 -g
CFLAGS		+=	-Wall $(INCS) $(DBGFLAGS)
LDFLAGS		+=	-lm
RM		=	rm -f

ifneq ($(wildcard /etc/debian_version),) 
	#Debian packaging
	ARCH	:=	$(shell dpkg-architecture -qDEB_BUILD_ARCH)
	DEB	:=	pewpewtris-$(VERSION)$(ARCH)
	PACKAGE	:=	$(DEB).deb
	SECTION	:=	games
	DEPS	=	libc6, libsdl1.2debian, libbz2-1.0, zlib1g
endif

ifeq ($(strip $(OS)), Windows_NT)
	LDFLAGS	+=	-lws2_32
else
ifeq ($(strip $(SBOX_UNAME_MACHINE)), arm)
	#Maemo packaging
	PACKAGE	:=	pewpewtris-$(VERSION)maemo.deb
	SECTION	:=	user/games
	DEPS	=	libc6, libsdl-gles1.2-1, libbz2-1.0, zlib1g
endif
endif
