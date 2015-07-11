import numpy as np 
from numpy.random import *
import math
import matplotlib.pyplot as plt
import copy

def generateData():
    count = 50
    data = [normal(-65,10) for i in xrange(count)]
    data.extend([normal(0,15) for i in xrange(count)])
    data.extend([normal(30,20) for i in xrange(count)])
    data.extend([normal(75,14) for i in xrange(count)])
    print len(data), data
    return data

def gussianProb(mean, std, x):
    return 1.0 / (std*math.sqrt(2*math.pi)) * math.exp(-(x-mean)**2/(2*std**2))


def split(mean, std, weight):
    # sigma = random()*30
    sigma = 100
    index = [i for i in xrange(len(std)) if std[i] == max(std)][0]
    u,sd,w = mean[index], std[index], weight[index]
    del mean[index]
    del std[index]
    del weight[index]
    mean.extend([u-sigma, u+sigma])
    std.extend([sd,sd])
    weight.extend([w/2.0,w/2.0])
    print 'current', mean, std, weight

    return mean, std, weight

def guassianMixtureProb(mean, std, weight, x):
    prob = 0
    for i in xrange(len(mean)):
        prob += weight[i]*gussianProb(mean[i], std[i], x)

    return prob

def weightProb(mean, std, weight, index, data):
    sumNumber, number = 0, 0
    for i in xrange(len(mean)):
        temp = weight[i] * gussianProb(mean[i], std[i], data)
        if i == index:
            number = temp
        sumNumber += temp

    return number / sumNumber

def computeWeight(data, mean, std, weight, index):
    total = 0
    for i in xrange(len(data)):
        total += weightProb(mean, std, weight, index, data[i])

    return total/len(data)

def computeMean(data, mean, std, weight, index):
    xSum, totalSum = 0, 0
    for i in xrange(len(data)):
        prob = weightProb(mean, std, weight, index, data[i])
        xSum += data[i] * prob
        totalSum += prob

    return xSum / totalSum

def computeStd(data, mean, std, weight, index, u):
    xSum, totalSum = 0, 0
    for i in xrange(len(data)):
        prob = weightProb(mean, std, weight, index, data[i])
        xSum += (data[i] - u)**2 * prob
        totalSum += prob

    return math.sqrt(xSum / totalSum)

def isConverge(mean, std, weight, newMean, newStd, newWeight):
    threshold = 0.01
    for i in xrange(len(mean)):
        if abs(mean[i]-newMean[i]) > threshold:
            return False
        if abs(std[i]-newStd[i]) > threshold:
            return False
        if abs(weight[i]-newWeight[i]) > threshold:
            return False

    return True

def splitAndBuild(data, mean, std, weight):
    mean, std, weight = split(mean, std, weight)
    newMean, newStd, newWeight = copy.deepcopy(mean), copy.deepcopy(std), copy.deepcopy(weight)

    while True:
        for i in xrange(len(mean)):
            newWeight[i] = computeWeight(data, mean, std, weight, i)
            newMean[i] = computeMean(data, mean, std, weight, i)
            newStd[i] = computeStd(data, mean, std, weight, i, newMean[i])
            # print 'result', mean[i], newMean[i]

        if isConverge(mean,std,weight,newMean,newStd,newWeight):
            return newMean, newStd, newWeight

        mean, std, weight = copy.deepcopy(newMean), copy.deepcopy(newStd), copy.deepcopy(newWeight)


def buildGuassianMixtureModel(data):
    mean = [np.mean(data)]
    std = [np.std(data)]
    weight = [1]
    while (len(mean) < 4):
        mean, std, weight = splitAndBuild(data, mean, std, weight)

    return mean, std, weight



data = generateData()
mean, std, weight = buildGuassianMixtureModel(data)
plt.figure()
plt.hist(data, len(data)/4, normed=1, facecolor='green', alpha=0.5)

for i in xrange(len(mean)):
    print mean[i],std[i],weight[i]

x = np.linspace(-100,100,4000)
y = [guassianMixtureProb(mean, std, weight, x[i]) for i in xrange(len(x))]
plt.figure()
plt.plot(x,y)
plt.show()




