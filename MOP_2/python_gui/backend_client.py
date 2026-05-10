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
        self.timeout = 2

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

            # Receive response
            response = b""
            self.socket.settimeout(self.timeout)  # Initial timeout
            
            while True:
                try:
                    chunk = self.socket.recv(4096)
                    if not chunk:
                        break
                    response += chunk

                    # Check for complete message (ends with newline)
                    if response.endswith(b"\n"):
                        break
                    
                    # For subsequent reads, use a shorter timeout (fast fail)
                    self.socket.settimeout(0.1)
                except socket.timeout:
                    # Timeout is OK - we've likely received everything
                    break
            
            result = response.decode('utf-8', errors='ignore').strip()
            return result
        except Exception as e:
            return None

    def startup_command(self, command):
        """Send a startup phase command (LD, SW, SR, SI, P)"""
        return self.send_command(command)

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

