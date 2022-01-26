#include <jack/jack.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc,char**argv)
{
  jack_client_t* jc1;
  jack_status_t stat;
  jack_port_t* port;
  int err;
  jc1 = jack_client_open("cli1",0,&stat);
  if( !jc1 ){
    fprintf(stderr,"jack_client_open failed with status %d\n",stat);
    return 1;
  }
  fprintf(stderr,"jack_client_open succeeded\n");
  sleep( 2 );
  // do something
  port = jack_port_by_name( jc1, "system:capture_1" );
  fprintf(stderr,"port type is \"%s\"\n", jack_port_type( port ) );
  sleep( 2 );
  err = jack_client_close( jc1 );
  fprintf(stderr,"jack_client_close returned %d\n",err);
  // end of first cycle, repeat:
  sleep( 1 );
  jc1 = jack_client_open("cli1",0,&stat);
  if( !jc1 ){
    fprintf(stderr,"jack_client_open failed with status %d\n",stat);
    return 1;
  }
  fprintf(stderr,"jack_client_open succeeded\n");
  sleep( 2 );
  // do something
  port = jack_port_by_name( jc1, "system:capture_1" );
  fprintf(stderr,"port type is \"%s\"\n", jack_port_type( port ) );
  sleep( 2 );
  err = jack_client_close( jc1 );
  fprintf(stderr,"jack_client_close returned %d\n",err);
  return 0;
}
