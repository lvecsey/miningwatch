
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct {

  char date[12];
  char sec_stamp[20];
  
  char flag;

  char program_name[30];

  char hashcode[12];

  float hashrate;
  
  char extra[20];
  
} row_t;

int run_reboot(char *ssh_host) {

  char cmd[240];

  int retval;
  
  fprintf(stderr, "%s: Executing system line for reboot.\n", __FUNCTION__);
  retval = sprintf(cmd, "ssh %s 'sudo /sbin/sysrq_boot'", ssh_host);    

  retval = system(cmd);
  exit(100);

  return 0;
  
}

int main(int argc, char *argv[]) {

  int retval;

  char *line = NULL;
  size_t len = 0;

  FILE *fp = stdin;

  long int lineno = 0;

  long int powhashno = 0;

  long int max_lines = 20;
  
  int debug = 0;

  char *ssh_host = argc>1 ? argv[1] : NULL;
  
  char *password = argc>2 ? argv[2] : NULL;

  row_t row;

  double avg_hashrate = 0.0;

  long int num_hashrates = 0;
  
  while ((retval = getline(&line,&len,fp)) != -1) {

    if (len > 0) {

      if (strstr(line, "Mining on PoWhash") != NULL) {

	// 2017-09-19 13:25:12.282968500   m  13:25:12|ethminer  Mining on PoWhash #b818a7e7 : 83.89MH/s [A1+0:R0+0:F0]
	
	retval = sscanf(line, "%s %s   %c  %s  Mining on PoWhash %s : %fMH/s %s", row.date, row.sec_stamp, &row.flag, row.program_name, row.hashcode, &row.hashrate, row.extra);

	if (retval == 7) {

	  avg_hashrate += row.hashrate;
	  num_hashrates++;
	  
	}
	
	powhashno++;

      }
      
      lineno++;

    }

    if (lineno == max_lines) break;
    
  }

  fclose(stdin);

  avg_hashrate /= num_hashrates;
  
  if (debug) {

    fprintf(stderr, "powhashno=%ld\n", powhashno);
    fprintf(stderr, "lineno=%ld\n", lineno);    
    fprintf(stderr, "avg_hashrate=%.02f\n", avg_hashrate);
    fprintf(stderr, "num_hashrates=%ld\n", num_hashrates);
    
  }
  
  if (powhashno < 2 || avg_hashrate < 20.0) {

    run_reboot(ssh_host);
    
  }
  
  return 0;

}
  
