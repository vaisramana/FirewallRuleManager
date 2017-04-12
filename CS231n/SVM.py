
import numpy as np

class SVM(object):
    def __init__(self):
        weight = np.random.randn(32*32*3+1,10)
        delat = 1


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
                if j!= trainLabels[i]:
                    margin = scores[j] - correct_class_score + self.delat
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

    def evaluate(testData, testLabels):
        correctNum = 0
        imageNum = testData.shape[0]
        classNum = testLabels.shape[1]

        for i in xrange(imageNum):
            scores = testData[i].dot(self.weight)
            if np.argmax(scores) == testLabels[i]:
                correctNum += 1
        return correctNum
        
"""
	def update_mini_batch(self, mini_batch, eta, lambda, delta):
        for x, y in mini_batch:
            loss, dW = self.SVM_loss(x, y, self.weight, lambda, delta)
            self.weight = self.weight - eta*dW

    def SGD(self, trainData, trainLabels, epochs, mini_batch_size, eta, lambda, delta, testData=None, testLabels=None):
        if test_data: n_test = len(test_data)
        n = len(training_data)
        ticks = time.time()
        for j in xrange(epochs):
            mini_batches = [
                training_data[k:k+mini_batch_size]
                for k in xrange(0, n, mini_batch_size)]
            for mini_batch in mini_batches:
                self.update_mini_batch(mini_batch, eta)
            deltaTicks = time.time() - ticks
            ticks = time.time()
            if testData and testLabels:
                print "Epoch {0}: {1} / {2} consuming {3}s".format(
                    j, self.evaluate(testData, testLabels), n_test, deltaTicks)
            else:
                print "Epoch {0} complete consuming {1}s".format(j, deltaTicks)
 """
    
    

