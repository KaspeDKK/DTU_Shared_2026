import tkinter as tk
from gui import YukonGUI


def main():
    root = tk.Tk()
    app = YukonGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()
