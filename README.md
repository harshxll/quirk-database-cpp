
# Quirk Database - README

## Overview
Quirk Database is a lightweight C++ database system with custom socket implementation supporting:
- Secure client-server messaging
- File transfer capabilities 
- Multi-client handling via fork()

## Project Structure
```
quirk-database/
├── utility/            # Core components
│   ├── server_util.h   # Server operations
│   ├── client_util.h   # Client operations  
│   ├── fun_util.h      # Shared utilities
│   └── imports.h       # Common definitions
├── server.cpp          # Server entry point
└── client.cpp          # Client entry point
```

## Building
### Server:
```bash
g++ -std=c++17 server.cpp -o server 
```

### Client:
```bash
g++ -std=c++17 client.cpp -o client 
```

## Usage
### Start Server:
```bash
./server 
```

### Connect Client:
```bash
./client
```

### Command Reference:
| Command | Action |
|---------|--------|
| `/data <filename>` | Initiate file transfer |
| `/quit` | Terminate connection |
| *Any other text* | Send as message |

## Key Features
- **File Transfer**: Secure binary transfers via `/data` command
- **Session Control**: Clean disconnection with `/quit`
- **Multi-Client**: Concurrent connections via forked processes

## Development Roadmap
1. User-defined schema system
2. Server-side data persistence
3. AES-256 encryption layer
4. Automated build system

## Contribution Guidelines
1. Use descriptive branch names (e.g. `feat/file-encryption`)
2. Maintain consistent code style
3. Test functionality before submitting PRs

*Note: License will be added upon project maturity.*
