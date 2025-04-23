#include "./imports.h"
#include "./util_fun.h"

using namespace Utility;

namespace Server{

#define MAXN (1 << 20)
char buffer[MAXN];




void exectute_command_server(string s, int socket, sockaddr_in their){
	if(s == "/quit"){
		printf("%s %d : (status = Left the chat) \n", inet_ntoa(their.sin_addr), ntohs(their.sin_port));
		close(socket);
		exit(0);
	}

	if(s == "/data"){
		receive_file(socket);
	}
}


int init(sockaddr_in addr, int backlog = 10){
	int socket_fd;
	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	int yes = 0;
	if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &yes, sizeof(yes)) == -1){
		perror("socketopt \n");
		exit(1);
	}

	if(bind(socket_fd, (sockaddr *) &addr, sizeof(sockaddr) ) == -1){
		perror("bind \n");
		exit(1);
	}

	if(listen(socket_fd, backlog) == -1){
		perror("listen \n");
		exit(1);
	}

	printf("Server started at %s %d \n", inet_ntoa(addr.sin_addr),  htons(addr.sin_port));

	return socket_fd;
}



void handle_client(int socket, struct sockaddr_in their){
		printf("New connection fron %s %d\n", inet_ntoa(their.sin_addr), ntohs(their.sin_port));
		printf("Process id %d \n", getpid());

		int num_bytes;
		stringstream ss;
		while(true){
			num_bytes = recv(socket, buffer, MAXN-1, 0);
			buffer[num_bytes] = '\0';
			
			string content = buffer;

			content = trim(content);
			if(commands.count(content)){
				exectute_command_server(content, socket, their);
				content = "";
				num_bytes = 0;
				continue;
			}

			if(num_bytes)printf("Message from %s %d : %s \n",inet_ntoa(their.sin_addr), ntohs(their.sin_port), buffer);
			char message[] = "Recieved content \n";
			if(num_bytes > 0 && send(socket, message, strlen(message), 0) == -1){

				perror("send \n");
				exit(1);
			}
		}
	
}


void run(int socket_fd){
	sockaddr_in their_add;
	while(true){
		int new_socket;
		socklen_t sz = sizeof(sockaddr);
		if((new_socket = accept(socket_fd, (sockaddr *) &their_add, &sz)) == -1){
			perror("new_socket");
			exit(1);
		}
		pid_t pid = fork();
		if(pid == 0){
			close(socket_fd);
			handle_client(new_socket, their_add);
			exit(0);
		}else if(pid > 0){
			close(new_socket);
		}else{
			perror("fork");
			close(new_socket);
		}
	}
	close(socket_fd);
	printf("Server offline \n");
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

}