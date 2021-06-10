#include "main.h"
#include "ict_sys.h"


int main()
{  
	dir_creat(); // Derectory creat
	setup(); // Setup Run
	signal(SIGINT, sig_Handler);
	timer_t timerID;
	createTimer(&timerID,10, 0);

	delay(1000); // 1s
	while (true)
	{
		loop(); //Setup Run	
	}
	return 0;
}
