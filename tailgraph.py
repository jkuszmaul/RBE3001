#!/usr/bin/python
import pylab
import math
import numpy

yoff = 5.62 * 25.4
armlen = 6. * 25.4

"""
def getT(t, a): # theta, joint len.
  cost = math.cos(t)
  sint = math.sin(t)
  R = numpy.matrix([[cost, -sint, 0],
                    [sint,  cost, 0],
                    [0   ,  0   , 1]])
  P = numpy.matrix([[1, 0, a],
                    [0, 1, 0],
                    [0, 0, 1]])
  T = R * P
  return T

def forwardkin(a, b):
  Ta = getT(a, lena)
  Tb = getT(b, lenb)
  Tab = Ta * Tb
  zero = numpy.matrix([[0],
                       [0],
                       [1]])
  posa = Ta * zero
  posb = Tab * zero
  return posa, posb
"""

def forwardkin(height, t1, t2, l1, l2):
  x1 = l1 * math.sin(t1)
  y1 = l1 * math.cos(t1) + height
  x2 = l2 * math.sin(t1 + t2) + x1
  y2 = l2 * math.cos(t1 + t2) + y1
  return x1, y1, x2, y2


def follow():
  pylab.show()
  with open("/dev/ttyUSB0") as f:
    scatx = []
    scaty = []
    while True:
      line = f.readline()
      line = line.split(' ')
      if (len(line) < 2):
        continue
      t1 = 0
      t2 = 0
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
      pylab.cla()
      pylab.axis([-300, 300, 0, 600])
      pylab.plot([0, 0, x1, x2], [0, yoff, y1, y2])
      scatx.append(xpred)
      scaty.append(ypred)
      pylab.scatter(scatx, scaty)
      pylab.draw()

#follow()

xall1 = []
yall1 = []
xall2 = []
yall2 = []
for i in range(-90, 90, 5):
  for j in range(-90, 0, 3):
    _, _, x2, y2 = forwardkin(yoff, i * math.pi / 180.0, j * math.pi / 180.0, armlen, armlen)
    xall1.append(x2)
    yall1.append(y2)
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
