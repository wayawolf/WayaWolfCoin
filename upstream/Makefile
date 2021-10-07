BASE_OS ?= ubuntu20
export BASE_OS

SUBDIRS := src/*
.PHONY:	force

all:	unarchive build

unarchve:
	./download.py

build:	${SUBDIRS}

${SUBDIRS}:
	${MAKE} -C $@ -f Makefile.cryptocoin

