#include "./imports.h"
#include "./config.h"

namespace Utility{





void send_file(int socket, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    char signal[] = "/data";
    send(socket, signal, strlen(signal), 0);

    size_t file_size = file.tellg();
    file.seekg(0);
    size_t ext_pos = filename.find_last_of('.');
    std::string extension = (ext_pos != std::string::npos) ? filename.substr(ext_pos) : "";

    std::ostringstream header_stream;
    header_stream << "FILE " << file_size << " " << extension << "\n";
    std::string header = header_stream.str();

    if (send(socket, header.c_str(), header.size(), 0) == -1) {
        perror("Header send failed");
        return;
    }

    std::vector<char> sbuffer(4096);
    while (file.read(sbuffer.data(), sbuffer.size())) {
        if (send(socket, sbuffer.data(), file.gcount(), 0) == -1) {
            perror("File content send failed");
            return;
        }
    }
    if (file.gcount() > 0) {
        if (send(socket, sbuffer.data(), file.gcount(), 0) == -1) {
            perror("Final chunk send failed");
        }
    }

    std::cout << "File sent successfully: " << filename << " \n";
}





void receive_file(int socket) {
    std::string header;
    char c;
    while (recv(socket, &c, 1, 0) == 1 && c != '\n') {
        header += c;
    }

    std::istringstream header_stream(header);
    std::string command;
    size_t file_size;
    std::string extension;
    
    header_stream >> command >> file_size >> extension;
    
    if (command != "FILE") {
        std::cerr << "Invalid file transfer command" << std::endl;
        return;
    }

    std::ofstream file("received_file" + extension, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create file" << std::endl;
        return;
    }

    std::vector<char> sbuffer(4096);
    size_t remaining = file_size;
    
    while (remaining > 0) {
        size_t chunk_size = std::min(sbuffer.size(), remaining);
        ssize_t received = recv(socket, sbuffer.data(), chunk_size, 0);
        
        if (received <= 0) {
            perror("File receive failed");
            return;
        }
        
        file.write(sbuffer.data(), received);
        remaining -= received;
    }

    std::cout << "File received successfully (" << file_size << " bytes)" << std::endl;
}


bool blank(char c){
	return c == '\t' || c == '\n' || c == ' ' || c == '\r';
}


string trim(string s, bool start = 1, bool end = 1){
	if(start) s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !blank(ch); }));
   	if(end) s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !blank(ch); }).base(), s.end());
   	return s;
}

string get_data(string img_path){
	FILE* in = freopen(img_path.c_str(), "rb", stdin);
	char c = '\0';
	string data = "";
	while(fread(&c, 1, 1, in) > 0){
		data += c;
	}

	fclose(in);
	return data;
}

void download_data(string img_data, string extension, string file_path = ".", string output_filename = "output"){
	file_path += '/' + output_filename + "." + extension;
	FILE* out = freopen(file_path.c_str(), "wb", stdout);
	fwrite(img_data.c_str(), 1, img_data.size(), out);
	fclose(out);
}

string get_extension(string img_path){
	int sz = img_path.size();
	--sz;
	string extension = "";
	while(sz >= 0 && blank(img_path[sz])) --sz;
	while(sz > 0 && img_path[sz] != '.') extension = img_path[sz--] + extension;
	if(extension.empty() || !supported.count(extension)){
		string err = "File path / name is not supported : " + extension;
		perror(err.c_str());
		exit(1);
	} 
	return extension;
}
}