import serial
import sys
from PyQt4 import QtGui, QtCore
from variables import variables

#SERIAL_PORT = "/dev/tty.usbserial-A700dY7k"
SERIAL_PORT = "/dev/tty.usbserial-A800eIxN"


class TuneinoWindow(QtGui.QWidget):
    def __init__(self, serialport):
        super(TuneinoWindow, self).__init__()
        self.spinboxes = []
        self.sliders = []
        self.serialport = serialport
        self.initUI()
        
    def initUI(self):
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.read_serial)
        self.timer.start(100)

        grid = QtGui.QGridLayout()
        grid.setColumnMinimumWidth(1, 300)
        self.setLayout(grid)    

        for n, variable in enumerate(variables):
            name, default_value, (min_value, max_value) = variable
            label = QtGui.QLabel(name.lower().replace("_", " "))

            spinbox = QtGui.QSpinBox()
            spinbox.setRange(min_value, max_value)
            self.spinboxes.append(spinbox)

            slider = QtGui.QSlider(1)
            slider.setRange(min_value, max_value)
            self.sliders.append(slider)
            slider.setSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding);

            spinbox.valueChanged.connect(lambda v, index=n: self.update(index, v))
            slider.valueChanged.connect(lambda v, spinbox=spinbox: spinbox.setValue(v))
            slider.setValue(default_value)

            grid.addWidget(label, n, 0, alignment=QtCore.Qt.AlignRight)
            grid.addWidget(slider, n, 1)
            grid.addWidget(spinbox, n, 2)
        
        self.setWindowTitle('Tuneino')    
        self.show()
        
    def update(self, index, value):
        self.sliders[index].setValue(value)
        line = "set %d %d\n" % (index, value)
        #print "out:", line,
        self.serialport.write(line)
    
    def read_serial(self):
        line = self.serialport.readline()
        while line:
            print "in: ", line,
            line = self.serialport.readline()

def main():
    app = QtGui.QApplication(sys.argv)
    #serialport = open("demo.txt", "w")
    serialport = serial.Serial(SERIAL_PORT, 115200)
    print "handshake", serialport.readline().strip()
    serialport.timeout = 0
    tuneino = TuneinoWindow(serialport)
    sys.exit(app.exec_())
    serialport.close()  

if __name__ == '__main__':
    main()
