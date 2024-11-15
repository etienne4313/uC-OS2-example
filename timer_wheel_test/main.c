#include <ucos_ii.h>

#define FUNC(a,b) static void a ## b(int arg) { \
	timing[arg] = get_monotonic_time(); \
}

int debug = 1;

static unsigned long offset = 0UL;
static unsigned long table[] = {4000, 600, 1400, 13000, 50000, 2000, 1900, 7400, 6400, 8800, 8000, 3300};
static unsigned long timing[12];

void die(int err, int line)
{
	fprintf(stderr, "DIE %d : %d\n", err, line);
	while(1){};
}

FUNC(w, 0);
FUNC(w, 1);
FUNC(w, 2);
FUNC(w, 3);
FUNC(w, 4);
FUNC(w, 5);
FUNC(w, 6);
FUNC(w, 7);
FUNC(w, 8);
FUNC(w, 9);
FUNC(w, 10);
FUNC(w, 11);

void (*f[12])(int) = {w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11};

static unsigned char stack1[STACK_SIZE];
static void t1(void *p)
{
	int x;
	unsigned long t;

	while(1){

		/*
		 * Validate the event callback time against the desired output
		 * So here we are loading 12 schedule in the future relative to 't'
		 */
		PRINT("\nT1 %ld\n", offset);
		memset(timing, 0, sizeof(timing));
		t = get_monotonic_time();
		for(x=0; x<12; x++){
			schedule_work(f[x], x, t + offset + table[x]);
			_delay_us(100);
		}

		/* Let all the event complete */
		OSTimeDlyHMSM(0,0,0,100);

		/*
		 * Then compare the real value against the expectation
		 */
		PRINT("\n");
		for(x=0; x<12; x++){
			PRINT("%ld ", timing[x] - t - offset - table[x]);
		}

		offset += 100;
	}
}

static unsigned char stack2[STACK_SIZE];
static void t2(void *p)
{
	OS_CPU_SR cpu_sr = 0;
	while (1) {
		/* This thread is faking some IRQ noise */
    	OS_ENTER_CRITICAL();
		_delay_us(50);
		OS_EXIT_CRITICAL();
	}
}

int main(void)
{
	OS_CPU_SR cpu_sr = 0;

	/* Don't let any IRQ come */
    OS_ENTER_CRITICAL();

	lib_init();
	PRINT("Entering main control loop\n");

	OSInit();

	OSTaskCreate(t1, "t1", (void *)&stack1[STACK_SIZE - 1], 1);
	OSTaskCreate(t2, "t2", (void *)&stack2[STACK_SIZE - 1], 2);

	/* IRQ are enabled when the first thread is started */
	OSStart();

	/* Never reach */
	DIE(-1);
	OS_EXIT_CRITICAL();
	return 0;
}

