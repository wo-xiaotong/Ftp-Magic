from tkinter import *

if __name__=="__main__":
	topWindow=Tk()
	user_lb=Label(topWindow,text="user")
	pass_lb=Label(topWindow,text="pass")
	user_lb.pack(),pass_lb.pack()
	sure_btn=Button(topWindow,text="connect")
	sure_btn.pack()

	topWindow.mainloop()
