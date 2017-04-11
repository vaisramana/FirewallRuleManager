
import tarfile
import cPickle
import time
import numpy as np

def load_CIFAR10(inputFileName=""):
    isTheFirstFile = True
    testFileName = 'cifar-10-batches-py/test_batch'
    tar = tarfile.open('./cifar-10-python.tar.gz')
    
    names = tar.getnames()
    #exclude non-data files 
    names.remove('cifar-10-batches-py/readme.html')
    names.remove('cifar-10-batches-py/batches.meta')
    names.remove('cifar-10-batches-py')
    #exclude test files 
    names.remove(testFileName)
    
    #extract train data and labels
    if inputFileName in names:
        tar.extract(inputFileName,path='./')
        dict = unpickle(inputFileName)
        trainData = dict['data']
        trainLabels = dict['labels']
        trainBatchLabels = dict['batch_label']
        trainFilenames = dict['filenames']
    else:
        for name in names:
            tar.extract(name,path='./')
            dict = unpickle(name)
            if isTheFirstFile == True:
                trainData = dict['data']
                trainLabels = dict['labels']
                trainBatchLabels = dict['batch_label']
                trainFilenames = dict['filenames']
                isTheFirstFile = False
            else:
                trainData = np.append(trainData,dict['data'],axis=0)
                trainLabels = np.append(trainLabels,dict['labels'],axis=0)
                trainBatchLabels += dict['batch_label']
                np.append(trainFilenames,dict['filenames'],axis=0)
    
    #extract test data and labels  
    tar.extract(testFileName,path='./')
    dict = unpickle(testFileName)
    testData  = dict['data']
    testLabels = dict['labels']
    testBatchLabels = dict['batch_label']
    testFilenames = dict['filenames']
    
    tar.close()
    return(trainData,trainLabels,trainBatchLabels,trainFilenames,\
           testData,testLabels,testBatchLabels,testFilenames)
    
def unpickle(file):
    with open(file, 'rb') as fo:
        dict = cPickle.load(fo)
    return dict

class NearestNeighbor(object):
    def __init__(self):
        pass
  
    def train(self, X, y):
        """ X is N x D where each row is an example. Y is 1-dimension of size N """
        # the nearest neighbor classifier simply remembers all the training data
        self.Xtr = X
        self.ytr = y
  
    def predict(self, X):
        """ X is N x D where each row is an example we wish to predict label for """
        num_test = X.shape[0]
        # lets make sure that the output type matches the input type
        Ypred = np.zeros(num_test)
    
        # loop over all test rows
        for i in xrange(num_test):
            
            # find the nearest training image to the i'th test image
            # using the L1 distance (sum of absolute value differences)
            distances = np.sum(np.abs(self.Xtr - X[i,:]), axis = 1)
            min_index = np.argmin(distances) # get the index with smallest distance
            Ypred[i] = self.ytr[min_index] # predict the label of the nearest example
    
        return Ypred



ticks = time.time()
trainData,trainLabels,trainBatchLabels,trainFilenames,testData,testLabels,testBatchLabels,testFilenames \
    = load_CIFAR10()
    #= load_CIFAR10('cifar-10-batches-py/data_batch_3')
print "data loading consuming %ds" %(time.time()-ticks)
print trainData.shape

ticks = time.time()
trainData_rows = trainData.reshape(trainData.shape[0], 32 * 32 * 3) 
testData_rows = testData.reshape(testData.shape[0], 32 * 32 * 3) 
nn = NearestNeighbor() 
nn.train(trainData_rows, trainLabels) 
testLabels_predict = nn.predict(testData_rows) 

print 'accuracy: %f' % ( np.mean(testLabels_predict == testLabels) )
print "taining loading consuming %ds" %(time.time()-ticks)

