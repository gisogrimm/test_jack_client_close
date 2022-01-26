#include <jack/jack.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc,char**argv)
{
  jack_client_t* jc;
  jack_status_t stat;
  int err;
  jc = jack_client_open("cli1",0,&stat);
  if( !jc ){
    fprintf(stderr,"jack_client_open failed with status %d\n",stat);
    return 1;
  }
  fprintf(stderr,"jack_client_open succeeded\n");
  sleep( 2 );
  err = jack_client_close( jc );
  fprintf(stderr,"jack_client_close returned %d\n",err);
  return 0;
}
