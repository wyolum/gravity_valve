#!/usr/bin/python3

print("Gravity Valve Pulse Interface") ## use python3!
import tkinter as tk
import serial

s0 = serial.Serial('/dev/ttyS0', baudrate=115200, timeout=1)
def send_msg(msg):
    s0.write(msg.encode('ascii','ignore'))
    print(s0.read(1000))

def set_rate(*a):
    rate = scale_rate.get()
    send_msg(f'{rate}')
    
def set_duty(*ignore):
    duty = scale_duty.get()
    send_msg(f'h{duty}')

def curry(f, *args):
    def out(*a):
        return f(*args)
    return out

root = tk.Tk()
frame = tk.Frame()

frame_rate = tk.Frame(frame)
scale_rate = tk.Scale(frame_rate, from_=10, to_=120, label='Rate')
scale_rate.set(60)
scale_rate.pack()
button_rate = tk.Button(frame_rate, text = "SetRate", command=set_rate)
button_rate.pack()
frame_rate.pack(side=tk.LEFT)

frame_duty = tk.Frame(frame)
scale_duty = tk.Scale(frame_duty, from_=1, to_=99, label='Duty')
scale_duty.set(50)
set_duty()
scale_duty.pack()
button_duty = tk.Button(frame_duty, text = "SetDuty", command=set_duty)
button_duty.pack()
frame_duty.pack(side=tk.LEFT)

frame.pack(side=tk.LEFT)

#scale_duty = tk.Scale(frame, from_=0, to_=100, label='Duty')
#scale_duty.pack(side=tk.LEFT)
#frame.pack(anchor=tk.CENTER)
#button_10 = tk.Button(root, text = "SetDuty", command=set_rate)
#button_10.pack(anchor = tk.CENTER)

button_stop = tk.Button(root, text = "Stop", command=curry(send_msg, '0'))
button_stop.pack(anchor = tk.CENTER)
button_open = tk.Button(root, text = "Open", command=curry(send_msg, '-1'))
button_open.pack(anchor = tk.CENTER)

label = tk.Label(root)
label.pack()

root.mainloop()
