# -*- coding: utf-8 -*-

import Tkinter as tk


height = 450
width  = 450
size_square = 50

def initWin(window):

	canvas = tk.Canvas(window,width=width,height=height)
	canvas.pack()

	return  canvas

def draw_grid(canvas):
	for i in range(0,10):
		if i%3 == 0:
			canvas.create_line(i*size_square, 0,i*size_square, size_square*9,width=3)
			canvas.create_line(0,i*size_square, size_square*9,i*size_square,width=3)
		else:
			canvas.create_line(i*size_square, 0,i*size_square, size_square*9,width=1)
			canvas.create_line(0,i*size_square, size_square*9,i*size_square,width=1)

def clear_grid(canvas):
	canvas.delete("all")
	draw_grid(canvas)


def draw_sudoku(canvas):
	canvas.create_text(size_square/2,size_square/2,text="5")

if __name__ == '__main__':
	
	window = tk.Tk()
	canvas = initWin(window)
	draw_grid(canvas)
	draw_sudoku(canvas)

	window.mainloop()

