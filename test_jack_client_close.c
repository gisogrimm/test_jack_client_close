#include <jack/jack.h>
#include <stdio.h>
#include <unistd.h>

void jc1_shutdown(void* arg)
{
  fprintf(stderr, "jc1 is shutting down.\n");
}

void jc2_shutdown(void* arg)
{
  fprintf(stderr, "jc2 is shutting down.\n");
}

int main(int argc, char** argv)
{
  jack_client_t* jc1;
  jack_client_t* jc2;
  jack_status_t stat;
  jack_port_t* port;
  int err;
  jc1 = jack_client_open("cli1", 0, &stat);
  if(!jc1) {
    fprintf(stderr, "jack_client_open failed with status %d\n", stat);
    return 1;
  }
  fprintf(stderr, "jack_client_open succeeded\n");
  jack_on_shutdown(jc1, jc1_shutdown, NULL);
  sleep(1);
  // first client is open
  jc2 = jack_client_open("cli2", 0, &stat);
  if(!jc2) {
    fprintf(stderr, "jack_client_open failed with status %d\n", stat);
    return 1;
  }
  fprintf(stderr, "jack_client_open succeeded\n");
  jack_on_shutdown(jc2, jc2_shutdown, NULL);
  sleep(1);
  // do something
  port = jack_port_by_name(jc1, "system:capture_1");
  fprintf(stderr, "port type is \"%s\"\n", jack_port_type(port));
  sleep(2);
  err = jack_client_close(jc1);
  fprintf(stderr, "jack_client_close(jc1) returned %d\n", err);
  // do something with jc2
  port = jack_port_by_name(jc2, "system:capture_1");
  fprintf(stderr, "port type jc2 is \"%s\"\n", jack_port_type(port));
  sleep(2);
  err = jack_client_close(jc2);
  fprintf(stderr, "jack_client_close(jc2) returned %d\n", err);
  // end of first cycle, repeat:
  sleep(1);
  jc1 = jack_client_open("cli1", 0, &stat);
  if(!jc1) {
    fprintf(stderr, "jack_client_open failed with status %d\n", stat);
    return 1;
  }
  fprintf(stderr, "jack_client_open succeeded\n");
  jack_on_shutdown(jc1, jc1_shutdown, NULL);
  sleep(2);
  // do something
  port = jack_port_by_name(jc1, "system:capture_1");
  fprintf(stderr, "port type is \"%s\"\n", jack_port_type(port));
  sleep(2);
  err = jack_client_close(jc1);
  fprintf(stderr, "jack_client_close returned %d\n", err);
  return 0;
}
