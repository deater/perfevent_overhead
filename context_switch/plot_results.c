#include <stdio.h>
#define NUM_KERNELS 17

char kernels[NUM_KERNELS][30]={
  "2.6.30",
  "2.6.31",
  "2.6.32",
  "2.6.33",
  "2.6.34",
  "2.6.35",
  "2.6.36",
  "2.6.37",
  "2.6.38",
  "2.6.39",
  "3.0.0",
  "3.1.0",
  "3.2.0",
  "3.3.0", 
  "3.4.0", 
  "3.5.0", 
  "3.5.0+", 
};

int main(int argc, char **argv) {
 
   int i,k;
   char filename[BUFSIZ],buffer[BUFSIZ];
   FILE *fff;
   int value;
   double min_value,max_value,latency,average;
   
   for(k=0;k<NUM_KERNELS;k++) {
      min_value=1e9;
      max_value=0.0;
      average=0.0;

      for(i=0;i<10;i++) {
	 sprintf(filename,"results/%s/0/results.%d",
		 kernels[k],i);
	 fff=fopen(filename,"r");
	 if (fff==NULL) continue;
	 
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 sscanf(buffer,"%d %lf",&value,&latency);
	 
	 fclose(fff);
	 if (latency<min_value) min_value=latency;
	 if (latency>max_value) max_value=latency;
	 average+=latency;
      }
      average/=10;
      printf("%d %lf %lf %lf (* %s *)\n",k,average,min_value,max_value,kernels[k]);
      
   }
   
   
   for(k=0;k<NUM_KERNELS;k++) {
      min_value=1e9;
      max_value=0.0;
      average=0.0;

      for(i=0;i<10;i++) {
	 sprintf(filename,"results/%s.perf/0/results.%d",
		 kernels[k],i);
	 fff=fopen(filename,"r");
	 if (fff==NULL) continue;
	 
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 sscanf(buffer,"%d %lf",&value,&latency);
	 
	 fclose(fff);
	 if (latency<min_value) min_value=latency;
	 if (latency>max_value) max_value=latency;
	 average+=latency;
      }
      average/=10;
      printf("%d %lf %lf %lf (* %s *)\n",k,average,min_value,max_value,kernels[k]);
      
   }
   
/* perfmon2 */
      min_value=1e9;
      max_value=0.0;
      average=0.0;

      for(i=0;i<10;i++) {
	 sprintf(filename,"results/2.6.30.perfmon/0/results.%d",i);
	 fff=fopen(filename,"r");
	 if (fff==NULL) continue;
	 
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 sscanf(buffer,"%d %lf",&value,&latency);
	 
	 fclose(fff);
	 if (latency<min_value) min_value=latency;
	 if (latency>max_value) max_value=latency;
	 average+=latency;
      }
      printf("%d %lf %lf %lf (* 2.6.30.perfmon *)\n",k,average,min_value,max_value);
   
   
   /* perfmon2 */
      min_value=1e9;
      max_value=0.0;
      average=0.0;

      for(i=0;i<10;i++) {
	 sprintf(filename,"results/2.6.30.perfmon-unused/0/results.%d",i);
	 fff=fopen(filename,"r");
	 if (fff==NULL) continue;
	 
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 sscanf(buffer,"%d %lf",&value,&latency);
	 
	 fclose(fff);
	 if (latency<min_value) min_value=latency;
	 if (latency>max_value) max_value=latency;
	 average+=latency;
      }
      printf("%d %lf %lf %lf (* 2.6.30.perfmon-unused *)\n",k,average,min_value,max_value);

   /* perfctr */
      min_value=1e9;
      max_value=0.0;
      average=0.0;

      for(i=0;i<10;i++) {
	 sprintf(filename,"results/2.6.32-perfctr/0/results.%d",i);
	 fff=fopen(filename,"r");
	 if (fff==NULL) continue;
	 
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 sscanf(buffer,"%d %lf",&value,&latency);
	 
	 fclose(fff);
	 if (latency<min_value) min_value=latency;
	 if (latency>max_value) max_value=latency;
	 average+=latency;
      }
      average/=10;
      printf("%d %lf %lf %lf (* 2.6.32-perfctr *)\n",k,average,min_value,max_value);

   /* perfctr */
      min_value=1e9;
      max_value=0.0;
      average=0.0;

      for(i=0;i<10;i++) {
	 sprintf(filename,"results/2.6.32-perfctr-unused/0/results.%d",i);
	 fff=fopen(filename,"r");
	 if (fff==NULL) continue;
	 
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 fgets(buffer,BUFSIZ,fff);
	 sscanf(buffer,"%d %lf",&value,&latency);
	 
	 fclose(fff);
	 if (latency<min_value) min_value=latency;
	 if (latency>max_value) max_value=latency;
	 average+=latency;
      }
      average/=10;
      printf("%d %lf %lf %lf (* 2.6.32-perfctr-unused *)\n",k,average,min_value,max_value);
      
   
   
   return 0;
}
