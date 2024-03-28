all: l2cap-client l2cap-server

l2cap-client: l2cap-client.c
	gcc -O3 -o l2cap-client l2cap-client.c -lbluetooth

l2cap-server: l2cap-server.c
	gcc -O3 -o l2cap-server l2cap-server.c -lbluetooth
