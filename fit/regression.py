import pandas
import matplotlib.pyplot as plt

def regression (dataX, dataY, a=1, b=1, rate=0.001, times=30):#y=ax+b
    for i in range (times):
        for k in range (len (dataX)):
            X = dataX[k]
            Y = dataY[k]
            a = a - rate * (a*X + b - Y) * X;
            b = b - rate * (a*X + b - Y);
            
    return a, b

def lineplot (a,b,fr=0,to=10,den=100): #plot y=ax+b
    import numpy as np
    x = np.linspace (fr,to,den)
    y = a*x+b
    plt.plot(x, y, '-r')
    
csv = pandas.read_csv ('dta.csv',delimiter=';')
print (csv)
plt.scatter (csv.iloc [:,0], csv.iloc[:,1])
a, b= regression (csv.iloc [:,0], csv.iloc[:,1])
lineplot (a,b)
plt.show ()
