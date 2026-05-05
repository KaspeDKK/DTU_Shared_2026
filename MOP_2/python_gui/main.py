"""
Yukon Solitaire GUI - Python tkinter frontend
Communicates with C backend via TCP/IP sockets
"""

import tkinter as tk
from tkinter import messagebox
from backend_client import BackendClient

class YukonGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Yukon Solitaire - GUI")
        # Bigger window so columns don't run out of space vertically
        self.root.geometry("1400x900")

        # Backend client
        self.backend = BackendClient()
        self.connected = False

        # Game state
        self.selected = None  # "C1", "C2:7H", etc.
        self.board_state = ""
        self.columns_data = [[] for _ in range(7)]  # Track card data for click detection
        self.foundations_data = ["", "", "", ""]  # Track foundation top cards for click detection
        
        # ---- Drawing/layout constants (initialized FIRST) ----
        self.CARD_W = 50
        self.CARD_H = 75
        self.COL_SPACING_X = 110
        self.COL_START_X = 25
        self.COL_START_Y = 70
        self.COL_Y_STEP = int(self.CARD_H * 0.40)  # 40% for 30px step - less overlap, more cards visible
        self.FOUND_START_X = self.COL_START_X + 7 * self.COL_SPACING_X + 30
        self.FOUND_START_Y = self.COL_START_Y
        self.FOUND_Y_STEP = self.CARD_H + 20

        # UI Elements (set up FIRST)
        self.setup_ui()

        # Connect to backend AFTER UI is ready
        self.connect_to_backend()

    def setup_ui(self):
        """Create UI components"""
        # Top bar with connection status and buttons
        top_frame = tk.Frame(self.root)
        top_frame.pack(pady=10)

        self.status_label = tk.Label(
            top_frame,
            text="Disconnected",
            font=("Arial", 12),
            fg="red"
        )
        self.status_label.pack(side=tk.LEFT, padx=10)

        tk.Button(top_frame, text="Start Game", command=self.start_game).pack(side=tk.LEFT, padx=5)
        tk.Button(top_frame, text="Refresh", command=self.refresh_board).pack(side=tk.LEFT, padx=5)
        tk.Button(top_frame, text="Quit", command=self.quit_game).pack(side=tk.LEFT, padx=5)

        # Main board area
        board_frame = tk.LabelFrame(self.root, text="Board", padx=10, pady=10)
        board_frame.pack(padx=10, pady=10, fill=tk.BOTH, expand=True)

        # Canvas for drawing the game board
        self.canvas = tk.Canvas(board_frame, width=1250, height=720, bg="darkgreen")
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.canvas.bind("<Button-1>", self.on_canvas_click)

        # ---- Drawing/layout constants (keep these in ONE place) ----
        self.CARD_W = 50
        self.CARD_H = 75
        self.COL_SPACING_X = 110
        self.COL_START_X = 25
        self.COL_START_Y = 70
        # How far down the next card starts.
        # Smaller number = more cards visible. 40% of card height = 30px per card.
        self.COL_Y_STEP = int(self.CARD_H * 0.40)

        # Foundations are drawn in their own vertical "column" on the right.
        self.FOUND_START_X = self.COL_START_X + 7 * self.COL_SPACING_X + 30
        self.FOUND_START_Y = self.COL_START_Y
        self.FOUND_Y_STEP = self.CARD_H + 20

        # Move controls
        move_frame = tk.LabelFrame(self.root, text="Make Move", padx=10, pady=10)
        move_frame.pack(padx=10, pady=5, fill=tk.X)

        tk.Label(move_frame, text="Source (e.g. C1, C3:7H):").pack(side=tk.LEFT, padx=5)
        self.source_entry = tk.Entry(move_frame, width=15)
        self.source_entry.pack(side=tk.LEFT, padx=5)

        tk.Label(move_frame, text="Destination (e.g. C4, F1):").pack(side=tk.LEFT, padx=5)
        self.dest_entry = tk.Entry(move_frame, width=15)
        self.dest_entry.pack(side=tk.LEFT, padx=5)

        tk.Button(move_frame, text="Move", command=self.manual_move).pack(side=tk.LEFT, padx=5)

        # Status area
        status_frame = tk.LabelFrame(self.root, text="Status", padx=10, pady=10)
        status_frame.pack(padx=10, pady=5, fill=tk.X)

        self.message_label = tk.Label(status_frame, text="Ready", font=("Arial", 11))
        self.message_label.pack()

    def connect_to_backend(self):
        """Connect to the C backend server"""
        if self.backend.connect():
            self.connected = True
            self.status_label.config(text="Connected", fg="green")
            self.update_message("Connected to backend")
        else:
            self.connected = False
            self.status_label.config(text="Disconnected", fg="red")
            self.update_message("Failed to connect to backend")

    def start_game(self):
        """Start a new game"""
        if not self.connected:
            messagebox.showerror("Error", "Not connected to backend")
            return

        response = self.backend.start_game()
        if response and "OK" in response:
            self.selected = None
            self.update_message("Game started!")
            self.refresh_board()
        else:
            messagebox.showerror("Error", f"Failed to start game: {response}")

    def refresh_board(self):
        """Refresh the board display"""
        if not self.connected:
            return

        response = self.backend.get_state()
        if response:
            if response.startswith("STATE|"):
                self.board_state = response[6:]  # Remove "STATE|" prefix
            self.draw_board()

    def draw_board(self):
        """Draw the game board with card graphics"""
        self.canvas.delete("all")

        # Draw green felt background (match current canvas size)
        self.canvas.update_idletasks()
        cw = max(1, self.canvas.winfo_width())
        ch = max(1, self.canvas.winfo_height())
        self.canvas.create_rectangle(0, 0, cw, ch, fill="darkgreen", outline="black", width=2)

        # Parse board and draw cards
        if self.board_state and self.board_state != "Game not started":
            self.draw_cards_from_state()
        else:
            self.canvas.create_text(
                600, 375,
                text="Game not started.\nClick 'Start Game'.",
                fill="white",
                font=("Arial", 14),
                justify="center"
            )

        # Display selected card info at bottom
        if self.selected:
            self.canvas.create_text(
                600, 720,
                text=f"Selected: {self.selected}",
                fill="yellow",
                font=("Arial", 10, "bold")
            )

    def draw_cards_from_state(self):
        """Parse board state and draw card graphics with overlapping"""
        lines = self.board_state.split('\n')
        if len(lines) < 2:
            return

        card_width = self.CARD_W
        card_height = self.CARD_H
        spacing_x = self.COL_SPACING_X
        y_step = self.COL_Y_STEP

        start_x = self.COL_START_X
        start_y = self.COL_START_Y

        # Draw column headers
        for col in range(7):
            x = start_x + col * spacing_x
            self.canvas.create_text(x + card_width//2, start_y - 15, text=f"C{col+1}", 
                                   fill="white", font=("Arial", 10, "bold"))

        # Draw foundations header (right side)
        foundations_x = self.FOUND_START_X
        self.canvas.create_text(foundations_x + (card_width * 0.5), start_y - 15,
                                text="FOUNDATIONS", fill="white",
                                font=("Arial", 10, "bold"))

        # Parse and draw cards from board state (skip header line)
        # We need to track cards per column to calculate overlapping positions
        columns = [[] for _ in range(7)]  # Store cards for each column: [(card_text, is_visible), ...]
        foundations = ["", "", "", ""]  # Store top card for each foundation

        import re

        for line_idx in range(1, len(lines)):
            line = lines[line_idx]
            if not line.strip():
                continue

            # Foundations can appear on the right side of the same lines as columns.
            # Collect whatever we can find, anywhere in the state.
            for f_idx in range(4):
                f_match = re.search(rf'F{f_idx+1}:\s*\[(.{{2}})\]', line)
                if f_match:
                    foundations[f_idx] = f_match.group(1).strip()

            # Parse tableau rows with visibility markers
            # Pattern: optional visibility marker (*=hidden, space=visible), then [rank suit]
            card_pattern = r'(\*| )\[(.{2})\]'
            card_matches = re.findall(card_pattern, line)
            if not card_matches:
                continue

            for col in range(min(7, len(card_matches))):
                vis_marker, card_text = card_matches[col]
                card_text = card_text.strip()
                if card_text and card_text != "  ":  # Skip empty slots
                    is_visible = (vis_marker == ' ')
                    columns[col].append((card_text, is_visible))
        
        # Store for click detection
        self.columns_data = columns
        self.foundations_data = foundations
        
        # Now draw cards with overlapping
        for col in range(7):
            for card_idx, (card_text, is_visible) in enumerate(columns[col]):
                x = start_x + col * spacing_x
                y = start_y + card_idx * y_step
                self.draw_card(x, y, card_text, card_width, card_height, is_visible)

        # Draw foundations as a vertical stack of slots on the right
        for f_idx in range(4):
            fx = self.FOUND_START_X
            fy = self.FOUND_START_Y + f_idx * self.FOUND_Y_STEP

            # Slot background (so empty foundations still look like a “place”)
            self.canvas.create_rectangle(
                fx, fy, fx + card_width, fy + card_height,
                fill="#1f7a1f", outline="gold", width=2
            )
            self.canvas.create_text(
                fx + 8, fy + 6, text=f"F{f_idx+1}",
                fill="gold", font=("Arial", 9, "bold"), anchor="nw"
            )

            top_card = foundations[f_idx] if f_idx < len(foundations) else ""
            if top_card:
                self.draw_card(fx, fy, top_card, card_width, card_height, True)
            else:
                self.canvas.create_text(
                    fx + card_width // 2,
                    fy + card_height // 2,
                    text="[  ]",
                    fill="gold",
                    font=("Consolas", 12, "bold")
                )

    def draw_card(self, x, y, card_text, width, height, is_visible=True):
        """Draw a single card with rank and suit, or card back if hidden"""
        if not is_visible:
            # Draw card back (hidden card)
            self.canvas.create_rectangle(x, y, x + width, y + height, 
                                        fill="#0a5c0a", outline="black", width=2)
            # Add cross pattern to indicate hidden
            self.canvas.create_line(x, y, x + width, y + height, fill="darkgray", width=1)
            self.canvas.create_line(x + width, y, x, y + height, fill="darkgray", width=1)
            return
        
        # Draw visible card
        # Determine suit color
        suit_char = card_text[1] if len(card_text) > 1 else ' '
        rank_char = card_text[0] if len(card_text) > 0 else ' '
        
        # Convert rank for display (T -> 10)
        rank_display = '10' if rank_char == 'T' else rank_char
        
        # Red suits: D (Diamonds), H (Hearts), Black suits: C (Clubs), S (Spades)
        is_red = suit_char in ['D', 'H']
        color = "red" if is_red else "black"
        
        # Draw card background (white)
        self.canvas.create_rectangle(x, y, x + width, y + height, 
                                    fill="white", outline="black", width=2)
        
        # Draw suit symbol in corner
        suit_symbols = {'D': '♦', 'H': '♥', 'C': '♣', 'S': '♠'}
        suit_display = suit_symbols.get(suit_char, '?')
        
        # Top-left corner
        self.canvas.create_text(x + 4, y + 2, text=suit_display, fill=color, 
                               font=("Arial", 10, "bold"), anchor="nw")
        self.canvas.create_text(x + 4, y + 12, text=rank_display, fill=color, 
                               font=("Arial", 7, "bold"), anchor="nw")
        
        # Bottom-right corner (upside down)
        self.canvas.create_text(x + width - 4, y + height - 2, text=suit_display, fill=color, 
                               font=("Arial", 10, "bold"), anchor="se")
        self.canvas.create_text(x + width - 4, y + height - 12, text=rank_display, fill=color, 
                               font=("Arial", 7, "bold"), anchor="se")

    def on_canvas_click(self, event):
        """Handle canvas clicks for card/column/foundation selection"""
        # Check if clicked on foundation area
        if event.x >= self.FOUND_START_X:
            # Foundation click
            found_col = (event.y - self.FOUND_START_Y) // self.FOUND_Y_STEP
            
            if 0 <= found_col < 4:
                card_text = self.foundations_data[found_col]
                if card_text and card_text.strip():  # Only if foundation has a card
                    if self.selected is None:
                        # Select foundation card as source
                        self.selected = f"F{found_col + 1}:{card_text}"
                        self.source_entry.delete(0, tk.END)
                        self.source_entry.insert(0, self.selected)
                        self.update_message(f"Selected: {self.selected}")
                        self.draw_board()
                    else:
                        # Move to foundation
                        destination = f"F{found_col + 1}"
                        self.try_move(self.selected, destination)
                        self.selected = None
                        self.draw_board()
            return
        
        # Tableau column click
        col = (event.x - self.COL_START_X) // self.COL_SPACING_X

        if col < 0 or col > 6:
            return

        # Determine which card was clicked based on Y coordinate
        card_idx = (event.y - self.COL_START_Y) // self.COL_Y_STEP
        
        # Get the column data
        if col >= len(self.columns_data) or card_idx >= len(self.columns_data[col]):
            # Click is below all cards or on empty column
            return
        
        # Get the actual card at this position
        card_text, is_visible = self.columns_data[col][card_idx]
        
        # First click: select source card
        if self.selected is None:
            self.selected = f"C{col + 1}:{card_text}"
            self.source_entry.delete(0, tk.END)
            self.source_entry.insert(0, self.selected)
            self.update_message(f"Selected: {self.selected}")
            self.draw_board()
        else:
            # Second click: attempt move to destination column
            destination = f"C{col + 1}"
            self.try_move(self.selected, destination)
            self.selected = None
            self.draw_board()

    def manual_move(self):
        """Make a move using manual entry"""
        source = self.source_entry.get().strip()
        dest = self.dest_entry.get().strip()

        if not source or not dest:
            messagebox.showwarning("Warning", "Please enter both source and destination")
            return

        self.try_move(source, dest)

    def try_move(self, source, destination):
        """Try to move a card"""
        if not self.connected:
            messagebox.showerror("Error", "Not connected to backend")
            return

        response = self.backend.make_move(source, destination)
        if response:
            if "OK" in response:
                self.update_message(f"Moved {source} -> {destination}")
                self.refresh_board()
                # Clear input fields after successful move
                self.source_entry.delete(0, tk.END)
                self.dest_entry.delete(0, tk.END)
            elif "ERROR" in response:
                self.update_message(f"Illegal move: {source} -> {destination}")
                messagebox.showwarning("Illegal Move", f"Cannot move {source} to {destination}")
            else:
                self.update_message(f"Response: {response}")
        else:
            messagebox.showerror("Error", "Communication failed")

    def update_message(self, msg):
        """Update the status message"""
        self.message_label.config(text=msg)

    def quit_game(self):
        """Quit the game"""
        if self.connected:
            self.backend.quit_game()
            self.backend.close()
        self.root.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = YukonGUI(root)
    root.mainloop()

