SUBDIRS := $(shell ls | grep -v Makefile | grep -v done)
DONEFILES := ${SUBDIRS:%=done.%}

all:	${SUBDIRS}

${SUBDIRS}:	%: done.%
	cat $@/* | (cd ../src/$@ && patch -p0)
	touch $<

${DONEFILES}:
