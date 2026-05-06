import re


def parse_board_state(board_state):
    """Parse board state string into structured data.

    Returns:
        columns: list of 7 lists of (card_text, is_visible) tuples
        foundations: list of 4 strings (top card or "")
    """
    columns = [[] for _ in range(7)]
    foundations = ["", "", "", ""]

    lines = board_state.split('\n')
    if len(lines) < 2:
        return columns, foundations

    for line in lines[1:]:
        if not line.strip():
            continue

        for f_idx in range(4):
            f_match = re.search(rf'F{f_idx + 1}:\s*\[(.{{2}})\]', line)
            if f_match:
                foundations[f_idx] = f_match.group(1).strip()

        card_matches = re.findall(r'(\*| )\[(.{2})\]', line)
        if not card_matches:
            continue

        for col in range(min(7, len(card_matches))):
            vis_marker, card_text = card_matches[col]
            card_text = card_text.strip()
            if card_text and card_text != "  ":
                columns[col].append((card_text, vis_marker == ' '))

    return columns, foundations
