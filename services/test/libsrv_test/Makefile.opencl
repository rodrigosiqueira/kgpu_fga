CC := gcc
FULLPATH := ../../../kgpu/include

ccflags-y += -I$(FULLPATH)
ccflags-y += -I$(FULLPATH)/gpuSpace
ccflags-y += -I$(FULLPATH)/kernelSpace
ccflags-y += -I$(FULLPATH)/userSpace
ccflags-y += -I$(FULLPATH)/utils

OPENCLMESSAGE := "-------------------- OpenCL -----------------"

all:
	@echo $(OPENCLMESSAGE)
	$(CC) --shared -fPIC -o libsrv_test.so srv_test.c $(ccflags-y) -lOpenCL
	$(if $(BUILD_DIR), cp libsrv_test.so $(BUILD_DIR)/ )

clean:
	rm -f *.o *.so
