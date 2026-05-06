import layout
from board_parser import parse_board_state


def draw_card(canvas, x, y, card_text, width, height, is_visible=True):
    """Draw a single card, or card back if hidden."""
    if not is_visible:
        canvas.create_rectangle(x, y, x + width, y + height,
                                fill="#0a5c0a", outline="black", width=2)
        canvas.create_line(x, y, x + width, y + height, fill="darkgray", width=1)
        canvas.create_line(x + width, y, x, y + height, fill="darkgray", width=1)
        return

    suit_char = card_text[1] if len(card_text) > 1 else ' '
    rank_char = card_text[0] if len(card_text) > 0 else ' '
    rank_display = '10' if rank_char == 'T' else rank_char
    color = "red" if suit_char in ('D', 'H') else "black"

    canvas.create_rectangle(x, y, x + width, y + height,
                            fill="white", outline="black", width=2)

    suit_symbols = {'D': '♦', 'H': '♥', 'C': '♣', 'S': '♠'}
    suit_display = suit_symbols.get(suit_char, '?')

    canvas.create_text(x + 4, y + 2, text=suit_display, fill=color,
                       font=("Arial", 10, "bold"), anchor="nw")
    canvas.create_text(x + 4, y + 12, text=rank_display, fill=color,
                       font=("Arial", 7, "bold"), anchor="nw")
    canvas.create_text(x + width - 4, y + height - 2, text=suit_display, fill=color,
                       font=("Arial", 10, "bold"), anchor="se")
    canvas.create_text(x + width - 4, y + height - 12, text=rank_display, fill=color,
                       font=("Arial", 7, "bold"), anchor="se")


def draw_startup_board(canvas, board_state):
    """Draw deck state as text in startup phase."""
    canvas.delete("all")
    canvas.update_idletasks()
    cw = max(1, canvas.winfo_width())
    ch = max(1, canvas.winfo_height())
    canvas.create_rectangle(0, 0, cw, ch, fill="darkgreen", outline="black", width=2)

    if board_state and board_state != "Game not started":
        canvas.create_text(50, 50, text="Deck Status:\n" + board_state[:800],
                           fill="white", font=("Arial", 10), justify="left", anchor="nw")
    else:
        canvas.create_text(
            600, 300,
            text="Startup Phase\n\nLoad a deck to begin:\n"
                 "  1. Click 'Load Deck (LD)'\n"
                 "  2. Optionally shuffle (SR) or split (SI)\n"
                 "  3. Click 'Play Game (P)' to start",
            fill="white", font=("Arial", 14), justify="center"
        )


def draw_startup_deck_columns(canvas, board_state):
    """Draw startup deck as graphical cards in 7 columns."""
    canvas.delete("all")
    canvas.update_idletasks()
    cw = max(1, canvas.winfo_width())
    ch = max(1, canvas.winfo_height())
    canvas.create_rectangle(0, 0, cw, ch, fill="darkgreen", outline="black", width=2)

    cards = []
    for token in board_state.replace("\n", " ").split():
        token = token.strip("[]")
        if len(token) == 2 and token[0] in "A23456789TJQK" and token[1] in "CDHS":
            cards.append(token)

    start_x = 30
    start_y = 60
    col_spacing = 100
    row_spacing = 35

    for col in range(7):
        x = start_x + col * col_spacing
        canvas.create_text(x + 20, start_y - 25, text=f"C{col + 1}",
                           fill="white", font=("Arial", 12, "bold"))

    for i, card in enumerate(cards):
        x = start_x + (i % 7) * col_spacing
        y = start_y + (i // 7) * row_spacing
        draw_card(canvas, x, y, card, layout.CARD_W, layout.CARD_H, True)

    f_x = start_x + 7 * col_spacing + 50
    for f in range(4):
        y = start_y + f * 110
        canvas.create_rectangle(f_x, y, f_x + layout.CARD_W, y + layout.CARD_H,
                                fill="#1f7a1f", outline="gold", width=2)
        canvas.create_text(f_x + layout.CARD_W // 2, y + layout.CARD_H // 2,
                           text=f"F{f + 1}", fill="gold", font=("Arial", 10, "bold"))


def draw_board(canvas, board_state, selected):
    """Draw the play phase board.

    Returns (columns_data, foundations_data) for click detection.
    """
    canvas.delete("all")
    canvas.update_idletasks()
    cw = max(1, canvas.winfo_width())
    ch = max(1, canvas.winfo_height())
    canvas.create_rectangle(0, 0, cw, ch, fill="darkgreen", outline="black", width=2)

    columns_data = [[] for _ in range(7)]
    foundations_data = ["", "", "", ""]

    if not board_state or board_state == "Game not started":
        canvas.create_text(600, 375, text="Game not started.\nClick 'Play Game'.",
                           fill="white", font=("Arial", 14), justify="center")
    else:
        columns_data, foundations_data = _draw_cards_from_state(canvas, board_state)

    if selected:
        canvas.create_text(600, 720, text=f"Selected: {selected}",
                           fill="yellow", font=("Arial", 10, "bold"))

    return columns_data, foundations_data


def _draw_cards_from_state(canvas, board_state):
    """Parse and render all cards on the play board.

    Returns (columns_data, foundations_data) for click detection.
    """
    columns, foundations = parse_board_state(board_state)

    for col in range(7):
        x = layout.COL_START_X + col * layout.COL_SPACING_X
        canvas.create_text(x + layout.CARD_W // 2, layout.COL_START_Y - 15,
                           text=f"C{col + 1}", fill="white", font=("Arial", 10, "bold"))

    canvas.create_text(layout.FOUND_START_X + layout.CARD_W * 0.5, layout.COL_START_Y - 15,
                       text="FOUNDATIONS", fill="white", font=("Arial", 10, "bold"))

    for col in range(7):
        x = layout.COL_START_X + col * layout.COL_SPACING_X
        if not columns[col]:
            canvas.create_rectangle(
                x, layout.COL_START_Y, x + layout.CARD_W, layout.COL_START_Y + layout.CARD_H,
                fill="#1a4d1a", outline="white", width=1, dash=(2, 2)
            )
            canvas.create_text(x + layout.CARD_W // 2, layout.COL_START_Y + layout.CARD_H // 2,
                               text="K only", fill="white", font=("Arial", 8), justify="center")
        else:
            for card_idx, (card_text, is_visible) in enumerate(columns[col]):
                y = layout.COL_START_Y + card_idx * layout.COL_Y_STEP
                draw_card(canvas, x, y, card_text, layout.CARD_W, layout.CARD_H, is_visible)

    for f_idx in range(4):
        fx = layout.FOUND_START_X
        fy = layout.FOUND_START_Y + f_idx * layout.FOUND_Y_STEP
        canvas.create_rectangle(fx, fy, fx + layout.CARD_W, fy + layout.CARD_H,
                                fill="#1f7a1f", outline="gold", width=2)
        canvas.create_text(fx + 8, fy + 6, text=f"F{f_idx + 1}",
                           fill="gold", font=("Arial", 9, "bold"), anchor="nw")

        if foundations[f_idx]:
            draw_card(canvas, fx, fy, foundations[f_idx], layout.CARD_W, layout.CARD_H, True)
        else:
            canvas.create_text(fx + layout.CARD_W // 2, fy + layout.CARD_H // 2,
                               text="[  ]", fill="gold", font=("Consolas", 12, "bold"))

    return columns, foundations
