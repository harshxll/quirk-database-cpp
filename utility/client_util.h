#include "./imports.h"
#include "./util_fun.h"

using namespace Utility;


namespace Client{


#define MAXN (1 << 20)

char buffer[MAXN];


void execute_command_client(string command, int socket = -1){
	size_t sz = command.size();
	if(command == "/quit"){
		send(socket, command.c_str(), sz, 0);
		printf("Connection closed for %d \n", getpid());
		close(socket);
		exit(0);
	}
	if(command == "/data"){
		cout << "Enter Relative filepath : ";
		string file; getline(cin, file);
		send_file(socket, file);
	}
}

sockaddr_in get_address(string domain = "localhost",int port=8080){
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	hostent* h = gethostbyname(domain.c_str());
	if(h == nullptr){
		perror("hostname\n");
		exit(1);
	}

	addr.sin_addr = *((in_addr *)(h->h_addr));
	addr.sin_port = htons(port);
	memset(&addr.sin_zero, '\0', 8);
	return addr; 
}

void print_addr(sockaddr_in my_add){
	printf("IP Address : %s \n", inet_ntoa(my_add.sin_addr));
	printf("Port : %d \n", ntohs(my_add.sin_port));
}

int init_client(int type = SOCK_STREAM){
	int socket_fd;
	if((socket_fd = socket(PF_INET, type, 0)) == -1){
		perror("socket");
		exit(1);
	}

	return socket_fd;
}

void connect(int sock,string domain = "localhost", int port = 8080){
	sockaddr_in their = get_address(domain, port);
	print_addr(their);
	if(connect(sock, (sockaddr *)&their, sizeof(sockaddr)) == -1){
		perror("connect");
		exit(1);
	}
}


bool input_handler(int sock){
	
	string content = "";
	while(true){
		cout << "Enter your Message : \t";
		getline(cin, content);  
		string command = trim(content);
		if(commands.count(command)){
			execute_command_client(command, sock);
			content = "";
			continue;
		}else{
			size_t sz = content.size();
			send(sock, content.c_str(), sz, 0);
		}
	}

	assert(false);
}


void run(int socket){
	input_handler(socket);
}
}
