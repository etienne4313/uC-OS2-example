#
# Build all the subdir
# 
#  ARCH avr:
#  Flash: make -C timer_wheel_test/ flash
#
#  KLM:
#  Load/unload: make -C timer_wheel_test/ load CPU=2; make -C timer_wheel_test/ unload CPU=2
#
PROGRAM := ping_pong_sem simple_sem timer_wheel_test timing_test

all:
	for n in $(PROGRAM); do $(MAKE) -C $$n || exit 1; done

clean:
	for n in $(PROGRAM); do $(MAKE) -C $$n clean; done

