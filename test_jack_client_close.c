#include <jack/jack.h>
#include <stdio.h>
#include <unistd.h>

struct timespec starttime;

/**
 * @fn timespec_diff(struct timespec *, struct timespec *, struct timespec *)
 * @brief Compute the diff of two timespecs, that is a - b = result.
 * @param a the minuend
 * @param b the subtrahend
 * @param result a - b
 *
 * from https://gist.github.com/diabloneo/9619917
 */
static inline void timespec_diff(struct timespec* a, struct timespec* b,
                                 struct timespec* result)
{
  result->tv_sec = a->tv_sec - b->tv_sec;
  result->tv_nsec = a->tv_nsec - b->tv_nsec;
  if(result->tv_nsec < 0) {
    --result->tv_sec;
    result->tv_nsec += 1000000000L;
  }
}

double sec_since_start()
{
  struct timespec cur;
  struct timespec diff;
  clock_gettime(CLOCK_REALTIME, &cur);
  timespec_diff(&cur, &starttime, &diff);
  return diff.tv_sec + 1e-9 * diff.tv_nsec;
}

void jc1_shutdown(void* arg)
{
  fprintf(stderr, "%1.6f jc1 is shutting down.\n", sec_since_start());
}

void jc2_shutdown(void* arg)
{
  fprintf(stderr, "%1.6f jc2 is shutting down.\n", sec_since_start());
}

int main(int argc, char** argv)
{
  jack_client_t* jc1;
  jack_client_t* jc2;
  jack_status_t stat;
  jack_port_t* port;
  int err;
  clock_gettime(CLOCK_REALTIME, &starttime);
  jc1 = jack_client_open("cli1", 0, &stat);
  if(!jc1) {
    fprintf(stderr, "%1.6f jack_client_open failed with status %d\n",
            sec_since_start(), stat);
    return 1;
  }
  fprintf(stderr, "%1.6f jack_client_open succeeded\n", sec_since_start());
  jack_on_shutdown(jc1, jc1_shutdown, NULL);
  err = jack_activate( jc1 );
  fprintf(stderr,"%1.6f jack_activate(jc1) returned %d\n", sec_since_start(),err);
  sleep(1);
  // first client is open
  jc2 = jack_client_open("cli2", 0, &stat);
  if(!jc2) {
    fprintf(stderr, "%1.6f jack_client_open failed with status %d\n",
            sec_since_start(), stat);
    return 1;
  }
  fprintf(stderr, "%1.6f jack_client_open succeeded\n", sec_since_start());
  jack_on_shutdown(jc2, jc2_shutdown, NULL);
  err = jack_activate( jc2 );
  fprintf(stderr,"%1.6f jack_activate(jc2) returned %d\n", sec_since_start(),err);
  sleep(1);
  // do something
  port = jack_port_by_name(jc1, "system:capture_1");
  fprintf(stderr, "%1.6f port type is \"%s\"\n", sec_since_start(),
          jack_port_type(port));
  sleep(1);
  err = jack_deactivate( jc1 );
  fprintf(stderr,"%1.6f jack_deactivate(jc1) returned %d\n", sec_since_start(),err);
  sleep(1);
  err = jack_client_close(jc1);
  fprintf(stderr, "%1.6f jack_client_close(jc1) returned %d\n",
          sec_since_start(), err);
  sleep(1);
  // do something with jc2
  port = jack_port_by_name(jc2, "system:capture_1");
  fprintf(stderr, "%1.6f port type jc2 is \"%s\"\n", sec_since_start(),
          jack_port_type(port));
  sleep(1);
  err = jack_deactivate( jc2 );
  fprintf(stderr,"%1.6f jack_deactivate(jc2) returned %d\n", sec_since_start(),err);
  sleep(1);
  err = jack_client_close(jc2);
  fprintf(stderr, "%1.6f jack_client_close(jc2) returned %d\n",
          sec_since_start(), err);
  // end of first cycle, repeat:
  sleep(1);
  jc1 = jack_client_open("cli1", 0, &stat);
  if(!jc1) {
    fprintf(stderr, "%1.6f jack_client_open failed with status %d\n",
            sec_since_start(), stat);
    return 1;
  }
  fprintf(stderr, "%1.6f jack_client_open succeeded\n", sec_since_start());
  jack_on_shutdown(jc1, jc1_shutdown, NULL);
  sleep(1);
  // do something
  port = jack_port_by_name(jc1, "system:capture_1");
  fprintf(stderr, "%1.6f port type is \"%s\"\n", sec_since_start(),
          jack_port_type(port));
  sleep(1);
  err = jack_client_close(jc1);
  fprintf(stderr, "%1.6f jack_client_close returned %d\n", sec_since_start(),
          err);
  return 0;
}
