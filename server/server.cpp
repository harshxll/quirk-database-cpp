#include "../utility/imports.h"
#include "../utility/server_util.h"

using namespace Server;


int main(){
	int socket_fd;
	sockaddr_in my_add = get_address("localhost", 3030);
	print_addr(my_add);
	socket_fd = init(my_add);
	run(socket_fd);
}