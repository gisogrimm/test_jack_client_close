# test_jack_client_close

Minimal test framework to expose `jack_client_close()` bug on macOS
Monterey.

To expose bug, start `jackd -d dummy`, and when jack is running,
`./test_jack_client_close`.

Expected output:

```
> ./test_jack_client_close 
jack_client_open succeeded
jack_client_close returned 0
```
