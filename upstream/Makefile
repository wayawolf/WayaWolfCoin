BASE_OS ?= ubuntu20
export BASE_OS

SUBDIRS := src/*
.PHONY:	force

all:	unarchive build

unarchive:
	./download.py

build:	${SUBDIRS}

${SUBDIRS}:	force
	${MAKE} -C $@ -f Makefile.cryptocoin

