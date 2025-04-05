from boofuzz import *
import socket
import time

SERVER_IP = "127.0.0.1"
SERVER_PORT = 4444

def fuzz_server():
    session = Session(
        target=Target(connection=TCPSocketConnection(SERVER_IP, SERVER_PORT))
    )

    s_initialize("FuzzedResponse")

    # Server will send a command like `whoami\n`, we just fuzz the response
    if s_block_start("response"):
        s_string("fake_response", fuzzable=True)
        s_delim("\n", fuzzable=True)
    s_block_end()

    session.connect(s_get("FuzzedResponse"))

    print("[*] Starting fuzzing session as fake client...")
    session.fuzz()

if __name__ == "__main__":
    fuzz_server()
