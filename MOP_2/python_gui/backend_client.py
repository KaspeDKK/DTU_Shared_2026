"""
Backend client for communicating with the C socket server
Handles TCP/IP communication with the Yukon Solitaire backend
"""

import socket
import sys

class BackendClient:
    def __init__(self, host="127.0.0.1", port=5000):
        self.host = host
        self.port = port
        self.socket = None
        self.timeout = 5

    def connect(self):
        """Connect to the backend server"""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(self.timeout)
            self.socket.connect((self.host, self.port))
            print(f"Connected to server at {self.host}:{self.port}")
            return True
        except Exception as e:
            print(f"Failed to connect: {e}")
            return False

    def send_command(self, command):
        """Send a command and receive response"""
        if self.socket is None:
            return None

        try:
            # Send command
            self.socket.sendall((command + "\n").encode())

            # Receive response (may be multi-line for board state)
            response = b""
            while True:
                try:
                    chunk = self.socket.recv(4096)
                    if not chunk:
                        break
                    response += chunk
                    # For simple responses, break after first chunk
                    if len(response) > 0 and b"\n" in response:
                        break
                except socket.timeout:
                    break
            
            return response.decode('utf-8', errors='ignore').strip()
        except Exception as e:
            print(f"Communication error: {e}")
            return None

    def start_game(self):
        """Start a new game"""
        return self.send_command("START")

    def get_state(self):
        """Get current board state"""
        return self.send_command("STATE")

    def make_move(self, from_col, to_col):
        """Make a move from one column/card to another"""
        cmd = f"MOVE {from_col}->{to_col}"
        return self.send_command(cmd)

    def quit_game(self):
        """Quit the game"""
        return self.send_command("QUIT")

    def close(self):
        """Close the connection"""
        if self.socket:
            self.socket.close()
            self.socket = None

if __name__ == "__main__":
    # Quick test
    client = BackendClient()
    if client.connect():
        print(client.start_game())
        print(client.get_state())
        client.close()

