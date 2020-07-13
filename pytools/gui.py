#!/usr/bin/env python
# -*- coding: utf-8 -*-
from tkinter import *
from tkinter import ttk
from tkinter import filedialog
import geister

kifu = []
with open('log.txt', 'r') as f:
    kifu = f.readlines()
    kifu = kifu[:-1]

lb = None
v1 = None
canvas = None
img = None
blue1 = None
red1 = None
blue2 = None
red2 = None
root = None

state_index = 0

game = None

nameList = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']

def show_state(state):
    canvas.delete('all')
    canvas.create_image(200,250,image=img)
    for i in range(16):
        x = int(state[i*3])
        y = int(state[i*3+1])
        xPos = x*54+65
        yPos = y*54+115
        nameY = y*54+100 if i < 8 else y*54+130
        if state[i*3+2] == 'B':
            if x < 6:
                canvas.create_image(xPos, yPos, image=blue1)
                canvas.create_text(xPos-15, nameY, text=nameList[i], font=("Consolas", 18, "bold"), fill='black')
            elif x == 8:
                canvas.create_image(-54+65, 115, image=blue1)
                canvas.create_text(-54+65, nameY, text=nameList[i])
        if state[i*3+2] == 'R':
            if x < 6:
                canvas.create_image(xPos, yPos, image=red1)
                canvas.create_text(xPos-15, nameY, text=nameList[i], font=("Consolas", 18, "bold"), fill='black')
            elif x == 8:
                canvas.create_image(-54+65, yPos, image=red1)
                canvas.create_text(-54+65, nameY, text=nameList[i])
        if state[i*3+2] == 'b':
            if x < 6:
                canvas.create_image(xPos, yPos, image=blue2)
                canvas.create_text(xPos-15, nameY, text=nameList[i], font=("Consolas", 18, "bold"), fill='black')
            elif x == 8:
                canvas.create_image(-54+65, yPos, image=blue2)
                canvas.create_text(-54+65, nameY, text=nameList[i])
        if state[i*3+2] == 'r':
            if x < 6:
                canvas.create_image(xPos, yPos, image=red2)
                canvas.create_text(xPos-15, nameY, text=nameList[i], font=("Consolas", 18, "bold"), fill='black')
            elif x == 8:
                canvas.create_image(-54+65, yPos, image=red2)
                canvas.create_text(-54+65, nameY, text=nameList[i])
    print(state)

def button_click():
    # show_selection()
    # canvas.create_line(1,1,100,100)
    x = root.winfo_pointerx()
    y = root.winfo_pointery()
    abs_coord_x = root.winfo_pointerx() - root.winfo_vrootx()
    abs_coord_y = root.winfo_pointery() - root.winfo_vrooty()
    wx, wy = x - root.winfo_x(), y - root.winfo_y()
    print('{}, {}. {}, {}. {}, {}'.format(x, y, abs_coord_x, abs_coord_y, wx, wy))
    
def listbox_selected(event):
    show_selection()
    state = lb.get(lb.curselection()[0]).rstrip('\n').split(',')[-1]
    show_state(state)
    
def show_selection():
    for i in lb.curselection():
        print(i, end=': ')
        print(lb.get(i).rstrip('\n'))

def donothing():
    pass

def openFile():
    global root
    global v1
    fn = filedialog.askopenfilename()#filetypes = typ, initialdir = dir)
    print(fn)
    root.title(fn)
    kifu = []
    with open(fn, 'r') as f:
        kifu = f.readlines()
        kifu = kifu[:-1]
    v1.set(kifu)
    # lb.update()
    # print(v1)

def motion(event):
    x, y = event.x, event.y
    print('{}, {}'.format(x, y))

def run():
    global lb
    global v1
    global canvas
    global img
    global blue1
    global red1
    global blue2
    global red2
    global root
    root = Tk()
    root.title('GUI for Geister')
    root.geometry('800x600')

    img = PhotoImage(file='./board.png')
    blue1 = PhotoImage(file='./blue1.png')
    red1 = PhotoImage(file='./red1.png')
    blue2 = PhotoImage(file='./blue2.png')
    red2 = PhotoImage(file='./red2.png')
    canvas = Canvas(root, bg="white", height=500, width=400)
    state = kifu[2].rstrip('\n').split(',')[-1]
    show_state(state)
    # label1 = Label(root, image=img, width=200)
    # label1.pack(side='left', ipadx=50, expand=True)
    # canvas.bind('<Button-1>', motion)
    canvas.pack(side='left', expand=True)
    
    # フレーム
    frame1 = ttk.Frame(root, padding=10)
    frame1.pack(side='right')
    
    # リストボックス
    v1 = StringVar(value=kifu)
    lb = Listbox(frame1, listvariable=v1,height=30,width=40)
    lb.bind('<<ListboxSelect>>', listbox_selected)
    lb.grid(row=0, column=0)

    #Button
    button1 = ttk.Button(frame1, text='OK', command=button_click, cursor="plus")
    button1.grid(row=0, column=1)

    menubar = Menu(root)
    filemenu = Menu(menubar, tearoff=0)
    filemenu.add_command(label="Open", command=openFile)
    filemenu.add_command(label="Save", command=donothing)
    filemenu.add_command(label="Save as...", command=donothing)
    filemenu.add_command(label="Close", command=donothing)
    filemenu.add_command(label="Exit", command=root.quit)
    menubar.add_cascade(label="File", menu=filemenu)
    filemenu = Menu(menubar, tearoff=0)
    filemenu.add_command(label="New", command=donothing)
    menubar.add_cascade(label="Game", menu=filemenu)

    # root.bind('<Motion>', motion)
    
    root.config(menu=menubar)
    root.mainloop()

if __name__ == '__main__':
    run()