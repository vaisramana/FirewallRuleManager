
import numpy as np
import time

class Softmax(object):
    def __init__(self):
        self.weight = np.random.randn(32*32*3+1,10)*0.0001
        #print "initial weight average is %f" %(self.weight.mean())


    def Softmax_loss(self, trainData, trainLabels, lmbd):
        """
        input N images 
        trainData : N*3073, one line represents one image
        trainLabels : N*10
        weight : 3073*10
        """

        #initialize the gradient as zero
        loss = 0.0
        weight = self.weight
        dW = np.zeros(weight.shape)

        imageNum = trainData.shape[0]
        classNum = weight.shape[1]
        
        for i in xrange(imageNum):
            f_i = np.dot(trainData[i,:],weight)
            log_C = np.max(f_i)
            f_i -= log_C

            sum_i = np.sum(np.exp(f_i))
            loss += -f_i[trainLabels[i]] + np.log(sum_i)
            for j in range(classNum):
                p = np.exp(f_i[j]) / sum_i
                dW[:,j] += (p - (j == trainLabels[i])) * trainData[i,:]

        """        
        #0...N-1
        for i in xrange(imageNum):
            #1*3073 dot 3073*10 = 1*10
            z = trainData[i,:].dot(weight)
            #make sure that exponent result will not overflow, z<0, e^z<1
            z -= np.max(z)
            #a=e^z/sum(e^z)
            a = np.exp(z)/np.sum(np.exp(z))
            #cross-entropy
            loss += np.sum(trainLabels[i]*np.log(a)+(1-trainLabels[i])*np.log(1-a))
            
            #1*10->10*1 x 1*3073 , update the whole 3073*10 weight matrix for every image
            for j in xrange(classNum):
                dW[:,j] += trainData[i]*((a-trainLabels[i])[j])
        """
        loss /= imageNum
        dW /= imageNum

        # Add regularization to the loss.
        loss += 0.5 * lmbd * np.sum(weight * weight)
        dW += lmbd*weight

        return loss, dW

    def predict(self, testData, testLabels):
        correctNum = 0
        imageNum = testData.shape[0]

        for i in xrange(imageNum):
            scores = testData[i].dot(self.weight)
            if np.argmax(scores) == testLabels[i]:
                correctNum += 1
        return correctNum


    def SGD(self, trainData, trainLabels, epochs, mini_batch_size, eta, lmbd, testData=None, testLabels=None):
        if testData is not None: 
            testNum = len(testData)
        n = len(trainData)
        ticks = time.time()
        for j in xrange(epochs):
            #something wrong with shuffle usage below
            #np.random.shuffle(trainData.T)
            for k in xrange(0, n, mini_batch_size):
                loss, dW = self.Softmax_loss(trainData[k:k+mini_batch_size], trainLabels[k:k+mini_batch_size], lmbd)
                self.weight = self.weight - eta*dW
            deltaTicks = time.time() - ticks
            ticks = time.time()
            if testData is not None and testLabels is not None:
                print "Epoch {0}: {1}/{2} on test data and {3}/{4} on train data consuming {5}s loss:{6}".format(
                    j, self.predict(testData, testLabels), testNum, 
                    self.predict(trainData, trainLabels), len(trainData), deltaTicks, loss)
                #print "weight average: %f" %(self.weight.mean())
            else:
                print "Epoch {0} complete consuming {1}s loss:{2}".format(j, deltaTicks, loss)


