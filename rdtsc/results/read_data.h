#define NUM_EVENTS 16
#define NUM_RUNS 1024
#define NUM_KERNELS 16

extern char kernel_names[NUM_KERNELS][64];
extern char colors[NUM_KERNELS][64];
extern long long times[NUM_KERNELS][NUM_EVENTS][NUM_RUNS];

#define PLOT_TYPE_START 0
#define PLOT_TYPE_STOP  1
#define PLOT_TYPE_READ  2
#define PLOT_TYPE_TOTAL 3

#define INTERFACE_PERF_EVENT 0
#define INTERFACE_PERFMON2   1
#define INTERFACE_PERFCTR    2
#define INTERFACE_PERF_EVENT_RDPMC 3
#define INTERFACE_UNKNOWN    99

struct cpuinfo_t {
   int vendor;
   int family;
   int model;
   int stepping;
   char modelname[BUFSIZ];
   char generic_modelname[BUFSIZ];
   int num_cpus;
};


int read_data(char *machine, int which, char *plot_name);
