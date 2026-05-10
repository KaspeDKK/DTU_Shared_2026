import tkinter as tk
from tkinter import messagebox, simpledialog
from backend_client import BackendClient
import drawing
import layout


class YukonGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Yukon Solitaire - GUI")
        self.root.geometry("1200x900")

        self.backend = BackendClient()
        self.connected = False

        self.game_phase = "STARTUP"
        self.selected = None
        self.board_state = ""
        self.columns_data = [[] for _ in range(7)]
        self.foundations_data = ["", "", "", ""]
        self.last_deck = ""

        self.setup_ui()
        self.connect_to_backend()

    def setup_ui(self):
        top_frame = tk.Frame(self.root)
        top_frame.pack(pady=10)

        self.status_label = tk.Label(top_frame, text="Disconnected",
                                     font=("Arial", 12), fg="red")
        self.status_label.pack(side=tk.LEFT, padx=10)

        tk.Button(top_frame, text="Refresh", command=self.refresh_board).pack(side=tk.LEFT, padx=5)
        tk.Button(top_frame, text="Quit", command=self.quit_game).pack(side=tk.LEFT, padx=5)

        board_frame = tk.LabelFrame(self.root, text="Board", padx=10, pady=10)
        board_frame.pack(padx=10, pady=10, fill=tk.BOTH, expand=True)

        self.canvas = tk.Canvas(board_frame, width=1100, height=600, bg="darkgreen")
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.canvas.bind("<Button-1>", self.on_canvas_click)

        self.controls_frame = tk.LabelFrame(self.root, text="Controls", padx=10, pady=10)
        self.controls_frame.pack(padx=10, pady=5, fill=tk.X)

        status_frame = tk.LabelFrame(self.root, text="Status", padx=10, pady=10)
        status_frame.pack(padx=10, pady=5, fill=tk.X)

        self.message_label = tk.Label(status_frame, text="Ready", font=("Arial", 11))
        self.message_label.pack()

    # ---- Connection ----

    def connect_to_backend(self):
        if self.backend.connect():
            self.connected = True
            self.status_label.config(text="Connected", fg="green")
            self.game_phase = "STARTUP"
            self.update_message("Connected to backend - Startup Phase")
            self.show_startup_controls()
            self.draw_startup_board()
        else:
            self.connected = False
            self.status_label.config(text="Disconnected", fg="red")
            self.update_message("Failed to connect to backend")

    # ---- Controls ----

    def show_startup_controls(self):
        self.game_phase = "STARTUP"
        for widget in self.controls_frame.winfo_children():
            widget.destroy()

        button_frame = tk.Frame(self.controls_frame)
        button_frame.pack(pady=5)

        tk.Button(button_frame, text="Load Deck (LD)",
                  command=self.startup_load_deck, width=15).pack(side=tk.LEFT, padx=3)
        tk.Button(button_frame, text="Show Deck (SW)",
                  command=self.startup_show_deck, width=15).pack(side=tk.LEFT, padx=3)
        tk.Button(button_frame, text="Shuffle (SR)",
                  command=self.startup_shuffle, width=15).pack(side=tk.LEFT, padx=3)
        tk.Button(button_frame, text="Split (SI)",
                  command=self.startup_split, width=15).pack(side=tk.LEFT, padx=3)
        tk.Button(button_frame, text="Save Deck (SD)",
                  command=self.startup_save_deck, width=15).pack(side=tk.LEFT, padx=3)

        button_frame2 = tk.Frame(self.controls_frame)
        button_frame2.pack(pady=5)
        tk.Button(button_frame2, text="Play Game (P)", command=self.startup_play,
                  width=20, bg="green", fg="white", font=("Arial", 11, "bold")).pack(side=tk.LEFT, padx=3)

    def show_play_controls(self):
        self.game_phase = "PLAY"
        for widget in self.controls_frame.winfo_children():
            widget.destroy()

        tk.Label(self.controls_frame,
                 text="Play Phase: Click cards to select/move or use manual entry",
                 font=("Arial", 10, "bold"), fg="green").pack()

        move_frame = tk.Frame(self.controls_frame)
        move_frame.pack(pady=5)

        tk.Label(move_frame, text="Source (e.g. C1, C3:7H):").pack(side=tk.LEFT, padx=5)
        self.source_entry = tk.Entry(move_frame, width=15)
        self.source_entry.pack(side=tk.LEFT, padx=5)

        tk.Label(move_frame, text="Destination (e.g. C4, F1):").pack(side=tk.LEFT, padx=5)
        self.dest_entry = tk.Entry(move_frame, width=15)
        self.dest_entry.pack(side=tk.LEFT, padx=5)

        tk.Button(move_frame, text="Move", command=self.manual_move).pack(side=tk.LEFT, padx=5)

        tk.Button(self.controls_frame, text="Leave Game", command=self.leave_game,
                  bg="orange", fg="white", font=("Arial", 10, "bold")).pack(pady=5)

    # ---- Startup commands ----

    def startup_load_deck(self):
        filename = simpledialog.askstring("Load Deck", "Enter filename:\n(Leave blank for default 'default')")
        if not filename or filename.strip() == "":
            filename = "default"
        filename = filename.strip()

        response = self.backend.startup_command(f"LD {filename}")
        if response and "OK" in response:
            self.last_deck = filename
            self.update_message(f"Deck loaded: {filename}")
            self.refresh_board()
        else:
            self.update_message("Deck load failed")
            messagebox.showerror("Error", f"Failed to load deck.\nResponse: {response}")

    def startup_show_deck(self):
        response = self.backend.startup_command("SW")
        if response:
            self.board_state = response[6:] if response.startswith("STATE|") else response
            self.update_message("Deck state displayed")
            self.draw_startup_deck_columns()

    def startup_shuffle(self):
        response = self.backend.startup_command("SR")
        if response and "OK" in response:
            self.update_message("Deck shuffled randomly")
            self.refresh_board()
        else:
            messagebox.showerror("Error", f"Failed to shuffle: {response}")

    def startup_split(self):
        split_val = simpledialog.askinteger("Split Deck", "Enter split position:\n1-51 for specific, 0 for random")
        if split_val is not None:
            response = self.backend.startup_command(f"SI {split_val}")
            if response and "OK" in response:
                self.update_message(f"Deck split at position {split_val}")
                self.refresh_board()
            else:
                messagebox.showerror("Error", f"Failed to split: {response}")

    def startup_save_deck(self):
        filename = simpledialog.askstring("Save Deck", "Enter filename to save to:\n(Leave blank for default 'cards')")
        if filename is None:
            return
        filename = filename.strip()
        command = f"SD {filename}" if filename else "SD"
        response = self.backend.startup_command(command)
        if response and "OK" in response:
            saved_as = filename if filename else "cards"
            self.update_message(f"Deck saved to {saved_as}.txt")
        else:
            messagebox.showerror("Error", f"Failed to save deck.\nResponse: {response}")

    def startup_play(self):
        response = self.backend.startup_command("P")
        if response and "OK" in response:
            self.selected = None
            self.update_message("Game started! Entering play phase...")
            self.show_play_controls()
            self.refresh_board()
        else:
            messagebox.showerror("Error", f"Failed to start game: {response}")

    def leave_game(self):
        if not self.last_deck:
            messagebox.showwarning("Warning", "No deck to return to")
            return
        response = self.backend.startup_command(f"LD {self.last_deck}")
        if response and "OK" in response:
            self.selected = None
            self.board_state = ""
            self.columns_data = [[] for _ in range(7)]
            self.foundations_data = ["", "", "", ""]
            self.update_message(f"Returned to startup — deck '{self.last_deck}' ready")
            self.show_startup_controls()
            self.draw_startup_board()
        else:
            messagebox.showerror("Error", f"Failed to reload deck: {response}")

    # ---- Board refresh and drawing ----

    def refresh_board(self):
        if not self.connected:
            return
        response = self.backend.get_state()
        if response:
            if response.startswith("STATE|"):
                self.board_state = response[6:]
            if self.game_phase == "STARTUP":
                self.draw_startup_board()
            else:
                self.draw_board()

    def draw_startup_board(self):
        drawing.draw_startup_board(self.canvas, self.board_state)

    def draw_startup_deck_columns(self):
        drawing.draw_startup_deck_columns(self.canvas, self.board_state)

    def draw_board(self):
        self.columns_data, self.foundations_data = drawing.draw_board(
            self.canvas, self.board_state, self.selected)

    # ---- Play phase interaction ----

    def on_canvas_click(self, event):
        if self.game_phase != "PLAY":
            self.update_message("Click cards during play phase")
            return

        if event.x >= layout.FOUND_START_X:
            found_col = (event.y - layout.FOUND_START_Y) // layout.FOUND_Y_STEP
            if 0 <= found_col < 4:
                card_text = self.foundations_data[found_col]
                if card_text and card_text.strip() and self.selected is None:
                    self.selected = f"F{found_col + 1}:{card_text}"
                    self.source_entry.delete(0, tk.END)
                    self.source_entry.insert(0, self.selected)
                    self.update_message(f"Selected: {self.selected}")
                    self.draw_board()
                elif self.selected is not None:
                    self.try_move(self.selected, f"F{found_col + 1}")
                    self.selected = None
                    self.draw_board()
            return

        col = (event.x - layout.COL_START_X) // layout.COL_SPACING_X
        if col < 0 or col > 6:
            return

        card_idx = (event.y - layout.COL_START_Y) // layout.COL_Y_STEP
        col_data = self.columns_data[col]

        if len(col_data) == 0:
            if card_idx != 0:
                return
            if self.selected is None:
                self.update_message("Empty column - select a source first")
                return
            self.try_move(self.selected, f"C{col + 1}")
            self.selected = None
            self.draw_board()
            return

        if card_idx >= len(col_data):
            return

        card_text, is_visible = col_data[card_idx]
        if not is_visible:
            self.update_message("Cannot select hidden card")
            return

        if self.selected is None:
            self.selected = f"C{col + 1}:{card_text}"
            self.source_entry.delete(0, tk.END)
            self.source_entry.insert(0, self.selected)
            self.update_message(f"Selected: {self.selected}")
            self.draw_board()
        else:
            self.try_move(self.selected, f"C{col + 1}")
            self.selected = None
            self.draw_board()

    def manual_move(self):
        if self.game_phase != "PLAY":
            messagebox.showwarning("Warning", "Can only move during play phase")
            return
        source = self.source_entry.get().strip()
        dest = self.dest_entry.get().strip()
        if not source or not dest:
            messagebox.showwarning("Warning", "Please enter both source and destination")
            return
        self.try_move(source, dest)

    def try_move(self, source, destination):
        if self.game_phase != "PLAY":
            return
        if not self.connected:
            messagebox.showerror("Error", "Not connected to backend")
            return

        response = self.backend.make_move(source, destination)
        if response:
            if "GAME_WON" in response:
                self.update_message("You won! Congratulations!")
                self.refresh_board()
                self.source_entry.delete(0, tk.END)
                self.dest_entry.delete(0, tk.END)
                messagebox.showinfo("Victory!", "You have successfully completed the game!\n\nCongratulations!")
            elif "OK" in response:
                self.update_message(f"Moved {source} -> {destination}")
                self.refresh_board()
                self.source_entry.delete(0, tk.END)
                self.dest_entry.delete(0, tk.END)
            elif "ERROR" in response:
                self.update_message(f"Illegal move: {source} -> {destination}")
                messagebox.showwarning("Illegal Move", f"Cannot move {source} to {destination}")
            else:
                self.update_message(f"Response: {response}")
        else:
            messagebox.showerror("Error", "Communication failed")

    # ---- Utility ----

    def update_message(self, msg):
        self.message_label.config(text=msg)

    def quit_game(self):
        if self.connected:
            self.backend.quit_game()
            self.backend.close()
        self.root.destroy()
