CC := gcc

all:
	$(CC) --shared -fPIC -o libsrv_test.so srv_test.c -lOpenCL
	$(if $(BUILD_DIR), cp libsrv_test.so $(BUILD_DIR)/ )

clean:
	rm -f *.o *.so
