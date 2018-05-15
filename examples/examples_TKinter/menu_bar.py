# -*- coding: utf-8 -*-

import Tkinter as tk


def open():
    print("Open !")

def save():
    print("Save !")

def solve():
    print("Solve !")

def initWin(window):
    menu_bar = tk.Menu(window)
    filemenu = tk.Menu(menu_bar)
    filemenu.add_command(label="Open", command=open)
    filemenu.add_command(label="Save", command=save)
    filemenu.add_command(label="Solve", command=solve)
    menu_bar.add_cascade(label="File", menu=filemenu)
    window.config(menu=menu_bar)


if __name__ == '__main__':

    window = tk.Tk()
    initWin(window)

    window.mainloop()

