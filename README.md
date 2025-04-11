# 🧠 C2 Infrastructure Project

A modular, extensible Command and Control (C2) system built in C++ with a strong emphasis on security, testability, and maintainability. Ideal for red team tooling, malware simulation, or advanced systems programming learning.

---

## 🔧 Features

- 🔁 **Reverse Shell Client** with reconnect logic, command execution, and resilience
- 🧭 **Singleton Configuration Manager** to load all runtime settings (e.g., port, buffer size, IP)
- 🗃️ **SQLite Integration** with Repository Pattern to manage client data cleanly
- 🧪 **GoogleTest-Based Unit Tests** for server and database operations
- 📡 **TCP Socket Interface** using abstracted `ISocket` interface
- ⚠️ **Graceful Error Handling** (e.g., SIGPIPE prevention, client disconnect recovery)

---

## 🗂️ Architecture Overview

```
C2Server
│
├── ConfigManager          # Loads config/env values (singleton)
├── ISocket / TcpSocket    # Abstracted socket interface
├── C2Server               # Main server loop
├── ClientRepository       # Handles SQLite DB operations
└── Logger                 # Console/file logging abstraction

ReverseShellClient
│
├── Reconnect loop         # Resilient to server drop
├── Dynamic buffer size    # Pulled from config
└── Command execution      # Sends back shell results
```

---

## ⚙️ Configuration

The project reads from a JSON or `.env`-style file like:

```json
{
  "server_ip": "127.0.0.1",
  "port": 4444,
  "buffer_size": 4096,
  "database": "c2.db"
}
```

---

## 🧪 Running Unit Tests

We use GoogleTest and GoogleMock for test coverage.

```bash
mkdir build && cd build
cmake ..
make tests
./tests
```

Tests include:
- Socket interaction mocking
- ConfigManager loading
- ClientRepository (CRUD)

---

## 🚀 Running the Project

```bash
# Compile server and client
make c2server
# Start the server
./c2server

---

## 📌 To-Do

- [ ] Session tracking & auditing
- [ ] Encrypted communication (TLS/AES)
- [ ] Admin control panel interface
- [ ] Plugin system for payload extensions
- [ ] Add enumeration in the client side to fill the client table

---

## 🧠 Inspiration
This project draws on offensive security operations, backdoor architecture, and production-ready C++ server design. It serves as both a learning tool and a foundational C2 platform.

---

## 🤝 Contributing
Contributions, feedback, and collaborations are welcome! Just fork, code, and PR. Feel free to open an issue if you have questions.
