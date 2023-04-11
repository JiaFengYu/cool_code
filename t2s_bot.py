# Jia Feng Yu
# Side Project: Text to Speech App


from tkinter import *
from gtts import gTTS
from playsound import playsound
import os

root = Tk()
root.geometry('350x300')
root.resizable(0,0)
root.config(bg = "#002b36")
root.title("Text to Speech")


Label(root, text = "Text to Speech" , font = "consolas 20 bold", bg = "#002b36", fg = "#17aaf9").pack()
Label(root, text = "This uses the Google Text To Speech API" , font = "consolas 12 bold", bg = "#002b36", fg = "#17aaf9").pack(side = BOTTOM)
Label(root, text = "Enter Text", font = "consolas 15 bold", bg = "#002b36", fg = "#17aaf9").place(x=20,y=60)


Msg = StringVar()
entry_field = Entry(root,textvariable = Msg, width ='50')
entry_field.place(x=20 , y=100)

def Text_to_speech():
    Message = entry_field.get()
    speech = gTTS(text = Message)
    speech.save("audio.mp3")
    playsound("audio.mp3")
    os.remove("audio.mp3")

def Exit():
    root.destroy()

def Reset():
    Msg.set("")

Button(root, text = "PLAY" , font = "consolas 15 bold", command = Text_to_speech, width = 4).place(x = 60, y=140)
Button(root, text = "EXIT", font = "consolas 15 bold", command = Exit, bg = "OrangeRed1").place(x = 135, y=140)
Button(root, text = "RESET", font = "consolas 15 bold", command = Reset).place(x = 210, y =140)

root.mainloop()
