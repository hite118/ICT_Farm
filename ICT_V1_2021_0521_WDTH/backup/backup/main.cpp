#include "main.h"
#include "ict_sys.h"
#include "am2315.h"

void timer_handler (int signum)
{
	float temperature, humidity;
	char *i2c_device = "/dev/i2c-1";
	int address = 0x5c;
	
        void *am = am2315_init(address, i2c_device);	
	if(am != NULL)
	{
		am2315_read_data(am, &temperature, &humidity);		
		
		printf("temperature = %0.2f, humidity = %0.2f\n", temperature, humidity);
		
		am2315_close(am);
	}
		
}


int main()
{  
	dir_creat(); // Derectory creat
	setup(); // Setup Run
	signal(SIGINT, sig_Handler);
	struct sigaction sa;
        
        struct itimerval timer;

        /* Install timer_handler as the signal handler for SIGVTALRM. */
        memset (&sa, 0, sizeof (sa));
        sa.sa_handler = &timer_handler;
        sigaction (SIGVTALRM, &sa, NULL);

        /* Configure the timer to expire after 250 msec... */
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 100000;

        /* ... and every 250 msec after that. */
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 100000;

        /* Start a virtual timer. It counts down whenever this process is executing. */
        setitimer (ITIMER_VIRTUAL, &timer, NULL);
	
	while (true)
	{
		loop(); //Setup Run	
	}
	return 0;
}
