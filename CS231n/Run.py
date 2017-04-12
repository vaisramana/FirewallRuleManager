
import CIFAR10_loader
import NearestNeighbor
import time
import numpy as np


#nearest neightbor classffier
ticks = time.time()
trainData,trainLabels,trainBatchLabels,trainFilenames,testData,testLabels,testBatchLabels,testFilenames \
    = CIFAR10_loader.load_CIFAR10('cifar-10-batches-py/data_batch_3')
    #= load_CIFAR10('cifar-10-batches-py/data_batch_3')
print "data loading consuming %ds" %(time.time()-ticks)
print trainData.shape

ticks = time.time()
trainData_rows = trainData.reshape(trainData.shape[0], 32 * 32 * 3) 
testData_rows = testData.reshape(testData.shape[0], 32 * 32 * 3) 
nn = NearestNeighbor.NearestNeighbor() 
nn.train(trainData_rows, trainLabels) 
testLabels_predict = nn.predict(testData_rows) 

print 'accuracy: %f' % ( np.mean(testLabels_predict == testLabels) )
print "taining loading consuming %ds" %(time.time()-ticks)





