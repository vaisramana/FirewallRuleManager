
import random
import numpy as np

class Network(object):
    def __init__(self, sizes):
        self.num_layers = len(sizes)
        self.sizes = sizes
        self.biases = [np.random.randn(y, 1) for y in sizes[1:]]
        self.weights = [np.random.randn(y, x) 
                        for x, y in zip(sizes[:-1], sizes[1:])]
                        	
def sigmoid(z):
    return 1.0/(1.0+np.exp(-z))
    

def feedforward(self, a):
    """Return the output of the network if "a" is input."""
    for b, w in zip(self.biases, self.weights):
        a = sigmoid(np.dot(w, a)+b)
    return a
    

net = Network([2,3,1])
print net.num_layers
print net.sizes
print net.biases
print net.weights
print net.weights[0][1]

