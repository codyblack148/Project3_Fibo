#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "fibonacci.h"
// These variables are used to record the accumulated times. They
// are set by the signal handlers and read by the processes when
// they report the results.
static long p_realt_secs = 0, c1_realt_secs = 0, c2_realt_secs = 0;
static long p_virtt_secs = 0, c1_virtt_secs = 0, c2_virtt_secs = 0;
static long p_proft_secs = 0, c1_proft_secs = 0, c2_proft_secs = 0;
static struct itimerval p_realt, c1_realt, c2_realt;
static struct itimerval p_virtt, c1_virtt, c2_virtt;
static struct itimerval p_proft, c1_proft, c2_proft;
//sigactions for every signal handler
static struct sigaction p_realt_sa, c1_realt_sa, c2_realt_sa;
static struct sigaction p_virtt_sa, c1_virtt_sa, c2_virtt_sa;
static struct sigaction p_proft_sa, c1_proft_sa, c2_proft_sa;

int main(int argc, char *argv[])
{
	long unsigned fib = 0;
	int pidl, pid2;
	unsigned int fibarg;
	int status,saCheck1,saCheck2,saCheck3,saCheck4,saCheck5,saCheck6,saCheck7,saCheck8,saCheck9;
	// Get command line argument, fibarg (the value N in the problem
	// statement)
  if(argc != 2)
{
  printf("Usage: project3 <integer n>\tn : index of the fibonacci sequence\n");
  exit(1);
}
else
{
  if(!(isdigit(argv[1][0])))
  {
    printf("Usage: ./fibonacci <integer n>\tn : must be an integer value\n");
    exit(-1);
  }
}
fibarg = atoi(argv[1]);

// Initialize parent, child 1, and child 2 timer values
p_realt.it_interval.tv_sec = SEC;
p_realt.it_interval.tv_usec = USEC;
p_realt.it_value.tv_sec = SEC;
p_realt.it_value.tv_usec = USEC;
p_virtt.it_interval.tv_sec = SEC;
p_virtt.it_interval.tv_usec = USEC;
p_virtt.it_value.tv_sec = SEC;
p_virtt.it_value.tv_usec = USEC;
p_proft.it_interval.tv_sec = SEC;
p_proft.it_interval.tv_usec = USEC;
p_proft.it_value.tv_sec = SEC;
p_proft.it_value.tv_usec = USEC;

//initialize sigactions so that there is no undefined behavior.
//set handlers to handler functions
memset(&p_realt_sa, 0, sizeof(p_realt_sa));
memset(&p_virtt_sa, 0, sizeof(p_virtt_sa));
memset(&p_proft_sa, 0, sizeof(p_proft_sa));
p_realt_sa.sa_handler = &p_realt_handler;
p_virtt_sa.sa_handler = &p_virtt_handler;
p_proft_sa.sa_handler = &p_proft_handler;

c1_realt.it_interval.tv_sec = SEC;
c1_realt.it_interval.tv_usec = USEC;
c1_realt.it_value.tv_sec = SEC;
c1_realt.it_value.tv_usec = USEC;
c1_virtt.it_interval.tv_sec = SEC;
c1_virtt.it_interval.tv_usec = USEC;
c1_virtt.it_value.tv_sec = SEC;
c1_virtt.it_value.tv_usec = USEC;
c1_proft.it_interval.tv_sec = SEC;
c1_proft.it_interval.tv_usec = USEC;
c1_proft.it_value.tv_sec = SEC;
c1_proft.it_value.tv_usec = USEC;

memset(&c1_realt_sa, 0, sizeof(c1_realt_sa));
memset(&c1_virtt_sa, 0, sizeof(c1_virtt_sa));
memset(&c1_proft_sa, 0, sizeof(c1_proft_sa));
c1_realt_sa.sa_handler = &c1_realt_handler;
c1_virtt_sa.sa_handler = &c1_virtt_handler;
c1_proft_sa.sa_handler = &c1_proft_handler;

c2_realt.it_interval.tv_sec = SEC;
c2_realt.it_interval.tv_usec = USEC;
c2_realt.it_value.tv_sec = SEC;
c2_realt.it_value.tv_usec = USEC;
c2_virtt.it_interval.tv_sec = SEC;
c2_virtt.it_interval.tv_usec = USEC;
c2_virtt.it_value.tv_sec = SEC;
c2_virtt.it_value.tv_usec = USEC;
c2_proft.it_interval.tv_sec = SEC;
c2_proft.it_interval.tv_usec = USEC;
c2_proft.it_value.tv_sec = SEC;
c2_proft.it_value.tv_usec = USEC;

memset(&c2_realt_sa, 0, sizeof(c2_realt_sa));
memset(&c2_virtt_sa, 0, sizeof(c2_virtt_sa));
memset(&c2_proft_sa, 0, sizeof(c2_proft_sa));
c2_realt_sa.sa_handler = &c2_realt_handler;
c2_virtt_sa.sa_handler = &c2_virtt_handler;
c2_proft_sa.sa_handler = &c2_proft_handler;

	// Enable parent's signal handlers
  saCheck1 = sigaction(SIGALRM, &p_realt_sa, NULL);
	saCheck2 = sigaction(SIGVTALRM, &p_virtt_sa, NULL);
	saCheck3 = sigaction(SIGPROF, &p_proft_sa, NULL);
	if(saCheck1 == -1 || saCheck2 == -1 || saCheck3 == -1){
		perror("error: ");
		exit(-5);
	}

	// Set parent's itimers
  if(setitimer(ITIMER_REAL, &p_realt, NULL) == -1)
		{
			perror("parent real timer set error");
			exit(-6);
		}
	if(setitimer(ITIMER_VIRTUAL, &p_virtt, NULL) == -1)
	{
		perror("parent real timer set error");
		exit(-6);
	}
	if(setitimer(ITIMER_PROF, &p_proft, NULL) == -1)
	{
		perror("parent real timer set error");
		exit(-6);
	}

	pidl = fork();

	if(pidl == 0)
	{
		// Enable child 1 signal handlers (disable parent handlers)
    saCheck4 = sigaction(SIGALRM, &c1_realt_sa, NULL);
    saCheck5 = sigaction(SIGVTALRM, &c1_virtt_sa, NULL);
    saCheck6 = sigaction(SIGPROF, &c1_proft_sa, NULL);
		if(saCheck4 == -1 || saCheck5 == -1 || saCheck6 == -1){
			perror("error: ");
			exit(-5);
		}
		// Set child 1 itimers
    if(setitimer(ITIMER_REAL, &c1_realt, NULL) == -1)
		{
			perror("c1 real timer set error");
			exit(-6);
		}
  	if(setitimer(ITIMER_VIRTUAL, &c1_virtt, NULL) == -1)
		{
			perror("c1 real timer set error");
			exit(-6);
		}
  	if(setitimer(ITIMER_PROF, &c1_proft, NULL) == -1)
		{
			perror("c1 real timer set error");
			exit(-6);
		}
		// Start child 1 on the fibonacci program
		fib = fibonacci(fibarg);

		// Read child 1 itimer values and report them
    if(getitimer(ITIMER_PROF, &c1_proft) == -1)
			{
				perror("child 1 real timer get error");
				exit(-6);
			}
		if(getitimer(ITIMER_REAL, &c1_realt) == -1)
		{
			perror("child 1 real timer get error");
			exit(-6);
		}
		if(getitimer(ITIMER_VIRTUAL, &c1_virtt) == -1)
		{
			perror("child 1 real timer get error");
			exit(-6);
		}
		printf("\n");
    printf ("Child 1 fib = %ld\nChild 1 real time = %ld sec, %ld msec\nChild 1 cpu time = %ld sec, %ld msec\nChild 1 user time = %ld sec, %ld msec\nChild 1 kernel time = %ld sec, %ld msec\n",
			fib, c1_realt_secs,
			  elapsed_usecs(c1_realt.it_value.tv_sec,
						  c1_realt.it_value.tv_usec) / 1000,
				c1_proft_secs,
				elapsed_usecs(c1_proft.it_value.tv_sec,
							  c1_proft.it_value.tv_usec) / 1000,
				c1_virtt_secs,
				elapsed_usecs(c1_virtt.it_value.tv_sec,
							  c1_virtt.it_value.tv_usec) / 1000,
				delta_time(c1_proft, c1_virtt,c1_realt_secs),
				(elapsed_usecs(c1_proft.it_value.tv_sec,
							   c1_proft.it_value.tv_usec) / 1000) -
				(elapsed_usecs(c1_virtt.it_value.tv_sec,
							   c1_virtt.it_value.tv_usec) / 1000));
		fflush(stdout);
		exit(0);
	}
	else
	{
		pid2 = fork();

		if(pid2 == 0)
		{
			// Enable child 2 signal handlers
      saCheck7 = sigaction(SIGALRM, &c2_realt_sa, NULL);
      saCheck8 = sigaction(SIGVTALRM, &c2_virtt_sa, NULL);
      saCheck9 = sigaction(SIGPROF, &c2_proft_sa, NULL);
			if(saCheck7 == -1 || saCheck8 == -1 || saCheck9 == -1){
				perror("error: ");
				exit(-5);
			}
			// Set child 2 itimers
      if(setitimer(ITIMER_REAL, &c2_realt, NULL) == -1)
			{
				perror("child 2 real timer set error");
				exit(-6);
			}
      if(setitimer(ITIMER_VIRTUAL, &c2_virtt, NULL) == -1)
			{
				perror("child 2 real timer set error");
				exit(-6);
			}
      if(setitimer(ITIMER_PROF, &c2_proft, NULL) == -1)
			{
				perror("child 2 real timer set error");
				exit(-6);
			}
			// Start child 2 on the fibonacci program
			fib = fibonacci(fibarg);
			// Read child 2 itimer values and report them
      if(getitimer(ITIMER_PROF, &c2_proft) == -1)
			{
				perror("c2 real timer get error");
				exit(-6);
			}
			if(getitimer(ITIMER_REAL, &c2_realt) == -1)
			{
				perror("c2 real timer get error");
				exit(-6);
			}
			if(getitimer(ITIMER_VIRTUAL, &c2_virtt) == -1)
			{
				perror("c2 real timer get error");
				exit(-6);
			}

        printf("\n");
			printf ("Child 2 fib = %ld\nChild 2 real time = %ld sec, %ld msec\nChild 2 cpu time = %ld sec, %ld msec\nChild 2 user time = %ld sec, %ld msec\nChild 2 kernel time = %ld sec, %ld msec\n",
			fib, c2_realt_secs,
			elapsed_usecs(c2_realt.it_value.tv_sec,
						  c2_realt.it_value.tv_usec) / 1000,
				c2_proft_secs,
				elapsed_usecs(c2_proft.it_value.tv_sec,
							  c2_proft.it_value.tv_usec) / 1000,
				c2_virtt_secs,
				elapsed_usecs(c2_virtt.it_value.tv_sec,
							  c2_virtt.it_value.tv_usec) / 1000,
				delta_time(c2_proft, c2_virtt,c2_realt_secs),
				(elapsed_usecs(c2_proft.it_value.tv_sec,
							   c2_proft.it_value.tv_usec) / 1000) -
				(elapsed_usecs(c2_virtt.it_value.tv_sec,
							   c2_virtt.it_value.tv_usec) / 1000));
			fflush(stdout);
			exit(0);
		}
		else
		{ 	/* this is the parent */

			// Start parent on the fibonacci program
			fib = fibonacci(fibarg);

			// Wait for children to terminate
			waitpid(0, &status, 0);
			waitpid(0, &status, 0);

			// Read parent itimer values and report them
      if(getitimer(ITIMER_PROF, &p_proft) == -1)
			{
				perror("parent real timer get error");
				exit(-6);
			}
      if(getitimer(ITIMER_REAL, &p_realt) == -1)
			{
				perror("parent real timer get error");
				exit(-6);
			}
      if(getitimer(ITIMER_VIRTUAL, &p_virtt) == -1)
			{
				perror("parent real timer get error");
				exit(-6);
			}
        printf("\n");
  			printf ("Parent fib = %ld\nParent real time = %ld sec, %ld msec\nParent cpu time = %ld sec, %ld msec\nParent user time = %ld sec, %ld msec\nParent kernel time = %ld sec, %ld msec\n",
  			fib, p_realt_secs,
  			elapsed_usecs(p_realt.it_value.tv_sec,
  						  p_realt.it_value.tv_usec) / 1000,
  				p_proft_secs,
  				elapsed_usecs(p_proft.it_value.tv_sec,
  							  p_proft.it_value.tv_usec) / 1000,
  				p_virtt_secs,
  				elapsed_usecs(p_virtt.it_value.tv_sec,
  							  p_virtt.it_value.tv_usec) / 1000,
  				delta_time(p_proft, p_virtt, p_realt_secs),
  				(elapsed_usecs(p_proft.it_value.tv_sec,
  							   p_proft.it_value.tv_usec) / 1000) -
  				(elapsed_usecs(p_virtt.it_value.tv_sec,
  							   p_virtt.it_value.tv_usec) / 1000));
  			fflush(stdout);
  			exit(0);

		}
			printf("this line should never be printed\n");
	}
}

long unsigned int fibonacci(unsigned int n)
{
	if(n == 0)
		return 0;
	else if (n == 1 || n == 2)
		return 1;
	else
		return(fibonacci(n-1) + fibonacci(n-2));
}
long unsigned int elapsed_usecs(long sec, long usec)
{
	if((sec*SEC_TO_MICRO)> usec)
			return ( SEC_TO_MICRO -( (sec*SEC_TO_MICRO) - usec));
	else
			return ( SEC_TO_MICRO -(usec - (sec*SEC_TO_MICRO) ));
}

long unsigned int delta_time(struct itimerval m, struct itimerval n,long realtime)
{
	if (realtime != 0)
	 	return (m.it_value.tv_sec - n.it_value.tv_sec)/realtime;
	else
		return m.it_value.tv_sec - n.it_value.tv_sec;
}
static void p_realt_handler(int signo)
{
		p_realt_secs++;
}

static void p_virtt_handler(int signo)
{
		p_virtt_secs++;
}
static void p_proft_handler(int signo)
{
		p_proft_secs++;
}

static void c1_realt_handler(int signo)
{
		c1_realt_secs++;
}

static void c1_virtt_handler(int signo)
{
		c1_virtt_secs++;
}
static void c1_proft_handler(int signo)
{
		c1_proft_secs++;
}
static void c2_realt_handler(int signo)
{
		c2_realt_secs++;
}

static void c2_virtt_handler(int signo)
{
		c2_virtt_secs++;
}
static void c2_proft_handler(int signo)
{
		c2_proft_secs++;
}
