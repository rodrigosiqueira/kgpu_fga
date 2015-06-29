# This makefile aim to call another subproject and put all together in the
# same folder.

BINFOLDER := build/
SUBPROJECT := kgpu services scripts

VISUALSEPARATOR := "------------------------------------------"

all: $(SUBPROJECT)
.PHONY: $(SUBPROJECT)

kgpucuda:
	@$(MAKE) $(SUBPROJECT)

kgpuopencl:
	@$(MAKE) $(SUBPROJECT)

# Call Makefile inside each folder.
$(SUBPROJECT): prepare
	@echo
	@echo $(VISUALSEPARATOR) $@ $(VISUALSEPARATOR)
	@echo
	$(MAKE) -C $@ $(TARGET) kv=$(kv) BUILD_DIR=`pwd`/build

# Preparation for compile

prepare:
	mkdir -p build
	mkdir -p build/module
	mkdir -p build/scripts

services: kgpu

# Clean rules
distclean:
	$(MAKE) all kv=$(kv) TARGET=clean

clean: distclean
	rm -rf build/*
