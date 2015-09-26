#!/usr/bin/python
import pylab
import math
import numpy

yoff = 5.62 * 25.4
armlen = 6. * 25.4

def forwardkin(height, t1, t2, l1, l2):
  """
    Compute the joint positions for a the provided thetas and link lengths.
    Parameters:
      height: The height of the initial link off the ground.
      t1, t2: The angles of the two angles, in radians.
      l1, l2: The lengths of the two links.
    Return:
      x1, y1, x2, y2: The x and y positions of links 1 and 2.
  """
  x1 = l1 * math.sin(t1)
  y1 = l1 * math.cos(t1) + height
  x2 = l2 * math.sin(t1 + t2) + x1
  y2 = l2 * math.cos(t1 + t2) + y1
  return x1, y1, x2, y2


def follow():
  """
    Actively tails the serial console and pdisplays a graph of the links.
  """
  #pylab.show()
  #pylab.ion()
  # The serial port by default writes lines to /dev/ttyUSB0 on this machine.
  #with open("/dev/ttyUSB0") as f:
  with open("/tmp/usb") as f:
    # Used to store all the end-point positions so traces of joint positions
    # can be recorded.
    scatx = []
    scaty = []
    for _ in range(3):
      line = f.readline()
      line = line.split(' ')
      # If the line is too short, it is obviously corrupted.
      if (len(line) < 2):
        continue
      t1 = 0
      t2 = 0
      # If any values fail to convert, throw out values and continue.
      try:
        t1 = int(line[0]) * math.pi / 180.0
        t2 = int(line[1]) * math.pi / 180.0
        xpred = float(line[2])
        ypred = float(line[3])
      except:
        continue
      x1, y1, x2, y2 = forwardkin(yoff, t1, t2, armlen, armlen)
      #print t1, t2
      #print x1, x2, y1, y2
      # Clear previous plot and replot everything.
      #pylab.cla()
      # Set axis bounds so that they don't change on every iteration.
      pylab.axis([-300, 300, 0, 600])
      pylab.plot([0, 0, x1, x2], [0, yoff, y1, y2])
      scatx.append(xpred)
      scaty.append(ypred)
      #pylab.scatter(scatx, scaty)
      pylab.draw()
    pylab.xlabel("X Position (mm)")
    pylab.ylabel("Y Position (mm)")
    pylab.title("Position of triangle vertices.")
    pylab.show()

# Comment/Uncomment in order to trail the serial port or not.
follow()

# Code for displaying the entire workspace of the arm.

# 1 and 2 refer to the two bucklings of the arm, so that it can be seen where
# on the plot there are two possible settings vs. just one.
xall1 = []
yall1 = []
xall2 = []
yall2 = []
for i in range(-90, 90, 5):
  for j in range(-90, 0, 3):
    _, _, x2, y2 = forwardkin(yoff, i * math.pi / 180.0, j * math.pi / 180.0, armlen, armlen)
    xall1.append(x2)
    yall1.append(y2)
  # Use an offset angle so that red/blue dots don't overlap exactly.
  for j in range(1, 90, 3):
    _, _, x2, y2 = forwardkin(yoff, i * math.pi / 180.0, j * math.pi / 180.0, armlen, armlen)
    xall2.append(x2)
    yall2.append(y2)

pylab.scatter(xall1, yall1, c='b', label="Left Buckling of arm")
pylab.scatter(xall2, yall2, c='r', label="Right Buckling of arm")
pylab.legend()
pylab.xlabel("X position, in mm; 0 is at base of arm")
pylab.ylabel("Y position, in mm; 0 is at base of arm")
pylab.title("Workspace of RBE3001 Arm")
pylab.show()
