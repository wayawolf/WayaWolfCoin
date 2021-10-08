SUBDIRS := $(shell ls | grep -v Makefile)

all:	${SUBDIRS}

${SUBDIRS}:	%: .done.%
	cat $@/* | (cd ../src/$@ && patch -p0)
	touch $<
