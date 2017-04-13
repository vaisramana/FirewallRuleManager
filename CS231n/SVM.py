
import numpy as np
import time

class SVM(object):
    def __init__(self):
        self.weight = np.random.randn(32*32*3+1,10)*1e4
        self.delta = 1
        print "initial weight average is %f" %(self.weight.mean())


    def SVM_loss(self, trainData, trainLabels, lmbd):
        """
        input N images 
        x : N*3073, one line represents one image
        y : N*10
        w : 3073*10
        """

        #initialize the gradient as zero
        loss = 0.0
        weight = self.weight
        dW = np.zeros(weight.shape)

        imageNum = trainData.shape[0]
        classNum = weight.shape[1]

        #0...N-1
        for i in xrange(imageNum):
            scores = trainData[i].dot(weight)#1*3073 dot 3073*10 = 1*10
            correct_class_score = scores[trainLabels[i]]#y[i] represents the right label of ith image
            #0...9
            for j in xrange(classNum):
                if j == trainLabels[i]:
                    continue
                margin = scores[j] - correct_class_score + self.delta
                if margin>0:
                    loss += margin
                    #calculate dw
                    dW[:,j] += trainData[i,:].T
                    dW[:,trainLabels[i]] -= trainData[i,:].T

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

    def update_weight(self, dW, eta):
        self.weight = self.weight - eta*dW
        

    def update_mini_batch(self, trainData, trainLabels, eta, lmbd):
        loss, dW = self.SVM_loss(trainData, trainLabels, lmbd)
        self.weight = self.weight - eta*dW

    def SGD(self, trainData, trainLabels, epochs, mini_batch_size, eta, lmbd, testData=None, testLabels=None):
        if testData is not None: 
            testNum = len(testData)
        n = len(trainData)
        ticks = time.time()
        for j in xrange(epochs):
            np.random.shuffle(trainData.T)
            for k in xrange(0, n, mini_batch_size):
                loss, dW = self.SVM_loss(trainData[k:k+mini_batch_size], trainLabels[k:k+mini_batch_size], lmbd)
                self.weight = self.weight - eta*dW
            deltaTicks = time.time() - ticks
            ticks = time.time()
            if testData is not None and testLabels is not None:
                print "Epoch {0}: {1}/{2} on test data and {3}/{4} on train data consuming {5}s loss:{6}".format(
                    j, self.predict(testData, testLabels), testNum, 
                    self.predict(trainData, trainLabels), len(trainData), deltaTicks, loss)
                print "weight average: %f" %(self.weight.mean())
            else:
                print "Epoch {0} complete consuming {1}s loss:{2}".format(j, deltaTicks, loss)


