# test_jack_client_close

Minimal test framework to expose `jack_client_close()` bug on macOS
Monterey. This is not about nice or clean code, the only purpose of
this project is to narrow down a bug.

To expose bug, start `jackd -d dummy`, and when jack is running,
`./test_jack_client_close`.

Expected output:

```
> ./test_jack_client_close 
0.027628 jack_client_open succeeded
0.027645 jack_set_process_callback(jc1,...) returned 0
0.045795 jack_activate(jc1) returned 0
1.069895 jack_client_open succeeded
1.069907 jack_set_process_callback(jc2,...) returned 0
1.091058 jack_activate(jc2) returned 0
2.091131 port type is "32 bit float mono audio"
3.095745 jack_deactivate(jc1) returned 0
3.095759 jc1_process was called 142 times
3.095761 jc2_process was called 94 times
4.098338 jack_client_close(jc1) returned 0
5.098498 port type jc2 is "32 bit float mono audio"
6.103697 jack_deactivate(jc2) returned 0
6.103711 jc1_process was called 142 times
6.103713 jc2_process was called 234 times
7.113845 jack_client_close(jc2) returned 0
8.134226 jack_client_open succeeded
9.134386 port type is "32 bit float mono audio"
10.164462 jack_client_close returned 0
10.164475 jc1_process was called 142 times
10.164478 jc2_process was called 234 times
```

Output on macOS 12.1:

```
% ./test_jack_client_close
0.029203 jack_client_open succeeded
0.029221 jack_set_process_callback(jc1,...) returned 0
0.044067 jack_activate(jc1) returned 0
1.069185 jack_client_open succeeded
1.069213 jack_set_process_callback(jc2,...) returned 0
1.089912 jack_activate(jc2) returned 0
2.094953 port type is "32 bit float mono audio"
3.114670 jack_deactivate(jc1) returned 0
3.114701 jc1_process was called 143 times
3.114705 jc2_process was called 95 times
Cannot read socket fd = 9 err = Socket is not connected
CheckRes error
JackMachSemaphore::TimedWait name = js501.cli1 usec = 9223372036854775807 err = (os/kern) object terminated
Cannot read socket fd = 7 err = Operation timed out
JackSocketClientChannel read fail
4.138975 jc1 is shutting down.
4.139055 jc1 is shutting down.
Could not read result type = 24
SuspendRefNum error
JackClient::Execute error name = cli1
4.139182 jack_client_close(jc1) returned -1
5.144213 port type jc2 is "32 bit float mono audio"
Server is not running
6.149328 jack_deactivate(jc2) returned -1
6.149338 jc1_process was called 143 times
6.149344 jc2_process was called 237 times
Server is not running
Cannot read socket fd = 11 err = Bad file descriptor
CheckRes error
JackSocketClientChannel read fail
7.154507 jc2 is shutting down.
7.164113 jack_client_close(jc2) returned -1
messagebuffer not initialized, skip message
messagebuffer not initialized, skip message
zsh: segmentation fault  ./test_jack_client_close
```

Output of `jackd -dummy` on macOS 12.1:

```
% jackd -d dummy
jackdmp 1.9.20
Copyright 2001-2005 Paul Davis and others.
Copyright 2004-2016 Grame.
Copyright 2016-2021 Filipe Coelho.
jackdmp comes with ABSOLUTELY NO WARRANTY
This is free software, and you are welcome to redistribute it
under certain conditions; see the file COPYING for details
JACK server starting in realtime mode with priority 10
self-connect-mode is "Don't restrict self connect requests"
JackMachSemaphore::Destroy failed to kill semaphore server thread...
JackMachSemaphoreServer::Execute: receive error - 268451846:(ipc/rcv) port changed
JackMachSemaphore::Destroy failed to kill semaphore server thread...
JackMachSemaphoreServer::Execute: receive error - 268451846:(ipc/rcv) port changed
```
