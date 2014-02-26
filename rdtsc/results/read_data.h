#define NUM_EVENTS 16
#define NUM_RUNS 1024

#define NUM_COLORFUL_KERNELS 16

#define NUM_KERNELS		23
#define NUM_RDPMC_KERNELS 	9
#define NUM_GCC_KERNELS		5
#define NUM_BREAKDOWN_KERNELS	8
#define NUM_FINAL_KERNELS	12
#define NUM_VARYING_KERNELS	4
#define NUM_MANYREAD_KERNELS	2

/*
#define KERNEL_2_6_30_PERFMON2 0
#define KERNEL_2_6_32_PERFCTR  1
#define KERNEL_2_6_32          2
#define KERNEL_3_5_0          16
#define KERNEL_3_5_0_RDPMC    17
*/

struct kernel_info {
    char name[64];
    int type;
};

extern struct kernel_info kernels[NUM_KERNELS];
extern struct kernel_info rdpmc_kernels[NUM_RDPMC_KERNELS];
extern struct kernel_info gcc_kernels[NUM_GCC_KERNELS];
extern struct kernel_info breakdown_kernels[NUM_BREAKDOWN_KERNELS];
extern struct kernel_info final_kernels[NUM_FINAL_KERNELS];
extern struct kernel_info varying_kernels[NUM_VARYING_KERNELS];
extern struct kernel_info manyread_kernels[NUM_MANYREAD_KERNELS];

extern char colors[NUM_KERNELS][64];
//extern long long times[NUM_KERNELS][NUM_EVENTS][NUM_RUNS];

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

#define STANDARD_KERNELS	0
#define RDPMC_KERNELS		1
#define GCC_KERNELS		2
#define BREAKDOWN_KERNELS	3
#define FINAL_KERNELS		4
#define VARYING_KERNELS		5
#define MANYREAD_KERNELS	6

int read_data(char *machine, int which, char *plot_name,
		int type, long long *times);
long long *get_runs(long long *pointer,int kernel, int event);
int sort_data(long long *times, int events, int type);
int calculate_boxplot_data(long long *times, int events,
			double *median, double *twentyfive,
			double *seventyfive,
			int type);
int calculate_deviation(long long *times, int events,
			double *average,double *deviation,int type);
int calculate_maxy(double *average, double *deviation, int type);

