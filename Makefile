#
# Build all the subdir
# Flash: make -C timer_wheel_test/ flash
#
PROGRAM := simple_sem timer_wheel_test timing_test

all:
	for n in $(PROGRAM); do $(MAKE) -C $$n || exit 1; done

clean:
	for n in $(PROGRAM); do $(MAKE) -C $$n clean; done

