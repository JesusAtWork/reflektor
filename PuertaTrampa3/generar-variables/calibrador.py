import serial
import sys
from PyQt4 import QtGui, QtCore
from variables import variables

#SERIAL_PORT = "/dev/tty.usbserial-A700dY7k"
SERIAL_PORT = "/dev/tty.usbserial-A800eIxN"


class TuneinoWindow(QtGui.QWidget):
    def __init__(self, serialport):
        super(TuneinoWindow, self).__init__()
	self.lcds = []
        self.serialport = serialport
        self.initUI()
        
    def initUI(self):
        self.timer = QtCore.QTimer()
	self.timer.timeout.connect(self.read_serial)
	#self.timer.start(100)

        vbox = QtGui.QVBoxLayout()
        vbox.addStretch(1)
        self.setLayout(vbox)    

	for n, variable in enumerate(variables):
            name, default_value, (min_value, max_value) = variable
            hbox = QtGui.QHBoxLayout()
            hbox.addStretch(1)
            vbox.addLayout(hbox)

            label = QtGui.QLabel(name)

            lcd = QtGui.QLCDNumber()
            self.lcds.append(lcd)

            #dial = QtGui.QDial()
            dial = QtGui.QSlider(1)
	    dial.setRange(min_value, max_value)
	    dial.valueChanged.connect(lambda v, index=n: self.update(index, v))
	    dial.setValue(default_value)
	    #self.update(n, control["inicial"])

            hbox.addWidget(label)
            hbox.addWidget(dial)
            hbox.addWidget(lcd)
        
        self.setWindowTitle('Tuneino')    
        self.show()
        
    def update(self, index, value):
	self.lcds[index].display(value)
        line = "set %d %d\n" % (index, value)
        self.serialport.write(line)
    
    def read_serial(self):
	line = self.serialport.readline()
	while line:
	    print line,
	    line = self.serialport.readline()

def main():
    app = QtGui.QApplication(sys.argv)
    serialport = open("demo.txt", "w")
    #serialport = serial.Serial(SERIAL_PORT, 115200)
    #print "handshake", serialport.readline().strip()
    #serialport.timeout = 0
    tuneino = TuneinoWindow(serialport)
    sys.exit(app.exec_())
    serialport.close()  

if __name__ == '__main__':
    main()

