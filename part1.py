import decimal
import numpy as np

function = lambda x: (-0.1*(x)**4)-(0.15*x**3)-(0.5*(x)**2)-(0.15*(x))+1.2
derivative = lambda x: (-0.4*(x)**3)-(0.45*(x)**2)-(x)-0.15
x = 0.5
h = 1
n = 10
stepsizeLst = [h]
differenceLst = [(function(x+h) - function(x-h))/(2*h)]
errorLst = [abs(derivative(x) - differenceLst[0])]

for i in range(0, n):
    h = h / n
    stepsizeLst.append(h)
    differenceLst.append((function(x+h)-function(x-h))/(2*h))
    errorLst.append(abs(derivative(x)-differenceLst[i]))
    print("{0} \t {1} \t {2}".format(decimal.Decimal(errorLst[i]), stepsizeLst[i], differenceLst[i]))

