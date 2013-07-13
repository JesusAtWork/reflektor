import os
import serial
import sys
import time
from copy import copy
from PyQt4 import QtGui, QtCore

SERIAL_PORT = ["/dev/" + o for o in os.listdir("/dev") if o.startswith("tty.usb")]
#CHARSET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ*abcdefghijklmnopqrstuvwxyz-#1234567890 "
CHARSET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ *-#1234567890<"

MAX_DISPLAY_DELAY = 150
MIN_DISPLAY_DELAY = 80
DISPLAY_ACCEL = 20
BLINK_SPEED = 300
DISPLAY_LEN = 5 

LEFT  = 1
RIGHT = 2
SPACE = 4


class TuneinoWindow(QtGui.QWidget):
    def __init__(self, serialport):
        super(TuneinoWindow, self).__init__()
        self.serialport = serialport
        self.teclas = {
            'space': False,
            'left': False,
            'right': False,
        }
        self.prev_teclas = copy(self.teclas)
        self.initials = [0] * DISPLAY_LEN
        self.cursor = 0
        self.last_change_time = time.time() * 1000
        self.last_b = -1
        self.initUI()
        
    def initUI(self):
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.loop)
        self.timer.start(10)

        grid = QtGui.QGridLayout()
        grid.setColumnMinimumWidth(1, 300)
        self.setLayout(grid)    
        self.label = QtGui.QLabel()
        self.update_display()

        font = self.label.font()
        font.setFamily("Monaco")
        font.setPointSize(72)
        font.setBold(True)
        self.label.setFont(font)

        grid.addWidget(self.label, 0, 0, alignment=QtCore.Qt.AlignRight)
        self.setWindowTitle('Tuneino')    
        self.show()

    def send_keys(self):
        b = 0
        if self.teclas["left"]:
            b += LEFT
        if self.teclas["right"]:
            b += RIGHT
        if self.teclas["space"]:
            b += SPACE
        if b != self.last_b:
            self.serialport.write(chr(b))
        self.last_b = b

    def keyPressEvent(self, e):
        if e.key() == QtCore.Qt.Key_Left:
            self.teclas["left"] = True
        if e.key() == QtCore.Qt.Key_Right:
            self.teclas["right"] = True
        if e.key() == QtCore.Qt.Key_Space:
            self.teclas["space"] = True
        self.send_keys()
        
    def keyReleaseEvent(self, e):
        if e.key() == QtCore.Qt.Key_Left:
            self.teclas["left"] = False
        if e.key() == QtCore.Qt.Key_Right:
            self.teclas["right"] = False
        if e.key() == QtCore.Qt.Key_Space:
            self.teclas["space"] = False
        self.send_keys()
        
    def update_display(self):
        text = [CHARSET[c] for c in self.initials]
        if int((time.time() * 1000 - self.last_change_time) / BLINK_SPEED) % 2:
            text[self.cursor] = "_"
        new_text = "".join(text)
        if self.label.text() != new_text:
            self.label.setText(new_text)
            #if self.serialport:
                #self.serialport.write(new_text + "\n")

    def change_display(self, d):
        self.initials[self.cursor] = (self.initials[self.cursor] + d) % len(CHARSET)

    def loop(self):
        if self.teclas != self.prev_teclas:
            self.display_delay = MAX_DISPLAY_DELAY
            self.prev_teclas = copy(self.teclas)

        if self.teclas["space"]:
            if not self.space_presionado:
                if self.initials[self.cursor] == CHARSET.index("<"):
                    self.initials[self.cursor] = CHARSET.index(" ")
                    self.cursor = max(0, self.cursor - 1)
                    self.initials[self.cursor] = CHARSET.index("<")
                else:
                    self.cursor = (self.cursor + 1) % len(self.initials)
                self.space_presionado = True
        else:
            self.space_presionado = False

        if self.teclas["left"] or self.teclas["right"]:
            if time.time() * 1000 > self.last_change_time + self.display_delay:
                self.last_change_time = time.time() * 1000
                if self.teclas["left"]:
                    self.change_display(-1)
                if self.teclas["right"]:
                    self.change_display(+1)
                self.display_delay = max(self.display_delay - DISPLAY_ACCEL, MIN_DISPLAY_DELAY)
        self.update_display()
        self.read_serial ();


    def read_serial(self):
        if not self.serialport:
            return
        line = self.serialport.readline()
        while line:
            print "in: ", line,
            line = self.serialport.readline()

def main():
    app = QtGui.QApplication(sys.argv)
    #serialport = open("demo.txt", "w")
    serialport = None
    if SERIAL_PORT:
        serialport = serial.Serial(SERIAL_PORT[0], 115200)
        #print "handshake", serialport.readline().strip()
        serialport.timeout = 0
    tuneino = TuneinoWindow(serialport)
    sys.exit(app.exec_())
    if serialport:
        serialport.close()  

if __name__ == '__main__':
    main()
