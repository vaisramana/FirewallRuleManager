
import tarfile
import cPickle

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

    
