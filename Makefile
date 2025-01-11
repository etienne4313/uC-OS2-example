#
# Build all the subdir
#
PROGRAM := simple_example ping_pong_sem simple_sem timer_wheel_test timing_test

#
# AVR arch
# Specify the program to be flashed
# make flash
# 
FLASH_PROGRAM := timing_test

all:
	for n in $(PROGRAM); do $(MAKE) -C $$n || exit 1; done

install:
	for n in $(PROGRAM); do $(MAKE) -C $$n install; done

debug:
	for n in $(PROGRAM); do $(MAKE) -C $$n debug; done

clean:
	for n in $(PROGRAM); do $(MAKE) -C $$n clean; done

flash:
	for n in $(FLASH_PROGRAM); do $(MAKE) -C $$n flash; done

mrproper:
	find . -name ".*.cmd" -type f -print0 | xargs -0 /bin/rm -f
	find . -name "*.ko" -type f -print0 | xargs -0 /bin/rm -f
	find . -name "*.o" -type f -print0 | xargs -0 /bin/rm -f
	rm -rf build

