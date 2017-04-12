
from CIFAR10_loader import load_CIFAR10
import NearestNeighbor
import SVM
import time
import numpy as np


#nearest neightbor classffier
ticks = time.time()
trainData,trainLabels,trainBatchLabels,trainFilenames,testData,testLabels,testBatchLabels,testFilenames \
    = load_CIFAR10('cifar-10-batches-py/data_batch_3')
    #= load_CIFAR10('cifar-10-batches-py/data_batch_3')
print "data loading consuming %ds" %(time.time()-ticks)
print trainData.shape

'''
ticks = time.time()
trainData_rows = trainData.reshape(trainData.shape[0], 32 * 32 * 3) 
testData_rows = testData.reshape(testData.shape[0], 32 * 32 * 3) 
nn = NearestNeighbor.NearestNeighbor() 
nn.train(trainData_rows, trainLabels) 
testLabels_predict = nn.predict(testData_rows) 

print 'accuracy: %f' % ( np.mean(testLabels_predict == testLabels) )
print "taining loading consuming %ds" %(time.time()-ticks)
'''
learning_rates = [1.4e-7, 1.5e-7, 1.6e-7]
regularization_strengths = [3e4, 3.1e4, 3.2e4, 3.3e4, 3.4e4]
svm = SVM.SVM()
eta = learning_rates[0]
lmbd = 100

#10000*3072 -> 10000*3073 add one more all-one line in the bottom
trainData = np.concatenate((trainData,np.ones((trainData.shape[0],1))),axis=1) 
testData = np.concatenate((testData,np.ones((testData.shape[0],1))),axis=1) 

"""
for i in xrange(1000):
    (loss, dW) = svm.SVM_loss(trainData, trainLabels, lmbd)
    svm.update_weight(dW, eta)
    correctNum = svm.evaluate(testData, testLabels)
    print "loss=%f correctNum=%d" %(loss,correctNum)
"""
svm.SGD(trainData, trainLabels, 30, 10, eta, lmbd, testData, testLabels)



