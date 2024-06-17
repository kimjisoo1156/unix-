#include "mytest.h"

int main() {

	create_source_data(); // one-time execution

	printf("Client_oriented_io time result\n");
	client_oriented_io();

	printf("Server_oriented_io time result\n");
	server_oriented_io();

	return 0;
}
