int main(int, char **);
long unsigned int fibonacci(unsigned int);
long unsigned int elapsed_usecs(long, long);
long unsigned int delta_time(struct itimerval, struct itimerval);
static void p_realt_handler(int);
static void p_virtt_handler(int);
static void p_proft_handler(int);
static void c1_realt_handler(int);
static void c1_virtt_handler(int);
static void c1_proft_handler(int);
static void c2_realt_handler(int);
static void c2_virtt_handler(int);
static void c2_proft_handler(int);
#define SEC 0
#define USEC 1
