# python program demonstrating 
# Combobox widget using tkinter 

#mport serial
import tkinter as tk 
from tkinter import ttk
import serial.tools.list_ports


# Creating tkinter window 
window = tk.Tk()
#logo = tk.PhotoImage(file="logo4.PNG")
#window.configure(bg='#87BECC')
window.title('KALYANI CENTER OF TECHNOLOGY AND INOVATION') 
window.geometry('580x380')

#functions
def hello():
    A=a.get()
    print("Data is")
    print(A)
    if A == '\0':
        A='0'
    B=b.get()
    if B==0X00:
        B='0'
    C=c.get()
    if C==0X00 :
        C='0'
    D=d.get()
    if D==0X00 :
        D='0'
    E=e.get()
    if E==0X00 :
        A='0'
    F=f.get()
    if F==0X00 :
        F='0'
    ttk.Label(window, text = '$'+','+A+','+B+','+C+','+D+','+E+','+F+','+'#',font = ("Robot", 10)).grid(column = 3, row = 10,columnspan=2,rowspan=2,sticky='W', padx = 10, pady = 25)
    #ser.write(b'$'+','+A+','+B+','+C+','+D+','+E+','+F+','+'#') 

def sel_usb():
    G=g.get()
    ttk.Label(window, text = "Connected to " + G + ' ' ,font = ("Robot", 10)).grid(column = 3, row = 11,columnspan=2,rowspan=2,sticky='W', padx = 10, pady = 25)
    

# label text for title

ttk.Label(window, text = "IF RECEIVER GUI",foreground ="black", font = ("Ubuntu Medium", 20)).grid(column = 1,row = 0, columnspan=2,sticky = 'N' ,padx = 0, pady = 30)
                             
#ttk.Label(window,image=logo).grid(column = 0,row = 0,sticky='E', padx = 0, pady = 10)
                             
                             
                             
# label
#labelframe = LabelFrame(window, text="This is a LabelFrame").grid(column = 0, 
#		row = 0, padx = 10, pady = 25)
#labelframe.pack(fill="both", expand="yes")
 
#left = Label(labelframe, text="Inside the LabelFrame")

ttk.Label(window, text = "ATTENUATOR 1 (dB):",
		font = ("Robot", 10)).grid(column = 0, 
		row = 6, padx = 10, pady = 25)

ttk.Label(window, text = "ATTENUATOR 2 (dB):",
		font = ("Robot", 10)).grid(column = 0, 
		row = 7, padx = 10, pady = 25)

ttk.Label(window, text = "ATTENUATOR 3 (dB):",
		font = ("Robot", 10)).grid(column = 0, 
		row = 8, padx = 10, pady = 25)



ttk.Label(window, text = "PHASE SHIFTER 1 (V):",
		font = ("Robot", 10)).grid(column = 2, 
		row = 6, padx = 10, pady = 25)

ttk.Label(window, text = "PHASE SHIFTER 2 (V):",
		font = ("Robot", 10)).grid(column = 2, 
		row = 7, padx = 10, pady = 25)

ttk.Label(window, text = "PHASE SHIFTER 3 (V):",
		font = ("Robot", 10)).grid(column = 2, 
		row = 8, padx = 10, pady = 25)

# Combobox creation 
a= tk.StringVar()
A1 = ttk.Combobox(window, width = 15, textvariable = a)

b= tk.StringVar()
A2 = ttk.Combobox(window, width = 15, textvariable = b)

c = tk.StringVar()
A3 = ttk.Combobox(window, width = 15, textvariable = c)



d = tk.StringVar()
PS1 = ttk.Combobox(window, width = 15, textvariable = d)

e = tk.StringVar()
PS2 = ttk.Combobox(window, width = 15, textvariable = e)

f = tk.StringVar()
PS3 = ttk.Combobox(window, width = 15, textvariable = f)


g= tk.StringVar()
port = ttk.Combobox(window,text="COM", width = 15, textvariable = g)


SEND = ttk.Button(window, text ="Send", command = hello)
connect = ttk.Button(window, text ="Connect", command = sel_usb)


# Adding combobox drop down list 
PS1['values'] = ('0','0.5','1.0','1.5','2.0','2.5','3.0','3.5','4.0','4.5','5.0','5.5','6.0','6.5','7.0','7.5','8.0','8.5','9.0','9.5','10.0','10.5','11.0','11.5','12.0','12.5','13.0','13.5','14.0','14.5','15.0')
PS2['values'] = ('0','0.5','1.0','1.5','2.0','2.5','3.0','3.5','4.0','4.5','5.0','5.5','6.0','6.5','7.0','7.5','8.0','8.5','9.0','9.5','10.0','10.5','11.0','11.5','12.0','12.5','13.0','13.5','14.0','14.5','15.0')
PS3['values'] = ('0','0.5','1.0','1.5','2.0','2.5','3.0','3.5','4.0','4.5','5.0','5.5','6.0','6.5','7.0','7.5','8.0','8.5','9.0','9.5','10.0','10.5','11.0','11.5','12.0','12.5','13.0','13.5','14.0','14.5','15.0')

A1['values']=('0','0.5','1.0','1.5','2.0','2.5','3.0','3.5','4.0','4.5','5.0','5.5','6.0','6.5','7.0','7.5','8.0','8.5','9.0','9.5','10.0','10.5','11.0','11.5','12.0','12.5','13.0','13.5','14.0','14.5','15.0','15.5','16.0','16.5','17.0','17.5','18.0','18.5','19.0','19.5','20.0','20.5','21.0','21.5','22.0','22.5','23.0','23.5','24.0','24.5','25.0','25.5','26.0','26.5','27.0','27.5','28.0','28.5','29.0','29.5','30.0','30.5','31.0'
)
A2['values']=('0','0.5','1.0','1.5','2.0','2.5','3.0','3.5','4.0','4.5','5.0','5.5','6.0','6.5','7.0','7.5','8.0','8.5','9.0','9.5','10.0','10.5','11.0','11.5','12.0','12.5','13.0','13.5','14.0','14.5','15.0','15.5','16.0','16.5','17.0','17.5','18.0','18.5','19.0','19.5','20.0','20.5','21.0','21.5','22.0','22.5','23.0','23.5','24.0','24.5','25.0','25.5','26.0','26.5','27.0','27.5','28.0','28.5','29.0','29.5','30.0','30.5','31.0'
)
A3['values']=('0','0.5','1.0','1.5','2.0','2.5','3.0','3.5','4.0','4.5','5.0','5.5','6.0','6.5','7.0','7.5','8.0','8.5','9.0','9.5','10.0','10.5','11.0','11.5','12.0','12.5','13.0','13.5','14.0','14.5','15.0','15.5','16.0','16.5','17.0','17.5','18.0','18.5','19.0','19.5','20.0','20.5','21.0','21.5','22.0','22.5','23.0','23.5','24.0','24.5','25.0','25.5','26.0','26.5','27.0','27.5','28.0','28.5','29.0','29.5','30.0','30.5','31.0'
)

#p['values'] = ( 'COM0','COM1','COM2','COM3')
port ['values'] = serial.tools.list_ports.comports()

A1.grid(column = 1, row = 6)
A2.grid(column = 1, row = 7)
A3.grid(column = 1, row = 8)

PS1.grid(column = 3, row = 6)
PS2.grid(column = 3, row = 7)
PS3.grid(column = 3, row = 8)

port.grid(column = 0, row = 10)

SEND.grid(column = 2, row = 10,)
connect.grid(column = 1, row = 10)

A1.current()
A2.current()
A3.current()

PS1.current()
PS2.current()
PS3.current()

#SEND.pack()
window.mainloop() 
