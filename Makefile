CC = gcc
CCFLAGS = -Wall -Werror -ggdb

compile_server:
	$(CC) $(CCFLAGS) tcp_echo_server.c -o Server
compile_client:
	$(CC) $(CCFLAGS) tcp_echo_client.c -o Client
run_server:
	./Server 3010
execute_client:
	./Client 0.0.0.0 exzia_noxianic_trai 3010

DBG = gdb
DFLAGS = --tui

debug_server:
	$(DBG) ./Server 3010
textually_debug_server:
	$(DBG) $(DFLAGS) ./Server 3010
debug_client:
	$(DBG) ./Client 0.0.0.0 ran_po_heir_lota 3010
textually_debug_client:
	$(DBG) $(DFLAGS) ./Client 0.0.0.0 iea_huera_lociaze_nos_me 3010

RMV = rm
RFLAGS = -f

clean:
	$(RMV) $(RFLAGS) Client Server
