BASE_OS ?= ubuntu20
export BASE_OS

SUBDIRS := src/*
.PHONY:	force

all:	unarchive build

unarchive:
	./download.py

patches:	force
	${MAKE} -C patches

build:	patches ${SUBDIRS}

${SUBDIRS}:	force
	${MAKE} -C $@ -f Makefile.cryptocoin

