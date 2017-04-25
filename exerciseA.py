import numpy as np
from os import path
import sys, os
import pandas as pd
import seaborn as sns
sns.set_color_codes()
import matplotlib
import matplotlib.pyplot as plt
plt.rc('text', usetex=True)

# Exp results
wind10 = np.array([[155,1.95],[154,1.95],[154,1.95]])
wind20 = np.array([[276,3.28],[277,3.27],[276,3.28]])
wind50 = np.array([[607,4.77],[607,4.78],[607,4.77]])
wind100 = np.array([[1054,5.01],[1051,5.02],[1051,5.02]])


if __name__ == '__main__':

    plt.figure(figsize=(6,4.5))
    p1 = plt.scatter(wind10[:,0],wind10[:,1],marker='+')
    p2 = plt.scatter(wind20[:,0],wind20[:,1],marker='x')
    p3 = plt.scatter(wind50[:,0],wind50[:,1],marker='*')
    p4 = plt.scatter(wind100[:,0],wind100[:,1],marker='d')
    plt.legend((p1,p2,p3,p4),('Window size = 10','Window size = 20',
        'Window size = 50','Window size = 100'))
    plt.xlabel('95th percentile signal delay (ms)')
    plt.ylabel('Throughput (Mbits/s)')
    #plt.show()
    ax = plt.gca()
    fig = ax.get_figure()
    plt.savefig('exerciseA.pdf')
    
    print "Mean value of score:"
    print np.mean(1000*wind10[:,1]/wind10[:,0])
    print np.mean(1000*wind20[:,1]/wind20[:,0])
    print np.mean(1000*wind50[:,1]/wind50[:,0])
    print np.mean(1000*wind100[:,1]/wind100[:,0])

    print "Variance of each run:"
    print np.var(1000*wind10[:,1]/wind10[:,0])
    print np.var(1000*wind20[:,1]/wind20[:,0])
    print np.var(1000*wind50[:,1]/wind50[:,0])
    print np.var(1000*wind100[:,1]/wind100[:,0])