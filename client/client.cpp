#include<../utility/imports.h>
#include <../utility/client_util.h>

using namespace Utility;
using namespace Client;


int main(int argc, char** argv){
	int socket_fd = init_client();	 
	connect(socket_fd,"localhost", 3030);
	run(socket_fd);

	return 0;
}
