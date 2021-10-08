SUBDIRS := $(shell ls | grep -v Makefile)
DONEFILES := ${SUBDIRS:%=done.%}

all:	${SUBDIRS}

${SUBDIRS}:	%: done.%
	cat $@/* | (cd ../src/$@ && patch -p0)
	touch $<

${DONEFILES}:
