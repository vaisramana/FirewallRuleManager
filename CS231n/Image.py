from scipy.misc import imread, imsave, imresize  
import numpy as np

def tint(image):
    # Read an JPEG image into a numpy array  
    img = imread(image)  
    #print img.dtype, img.shape  # Prints "uint8 (400, 248, 3)"  

    if './' in image:
        image = image.split('./')[1]
    imsave(image.split('.')[0]+"_copy."+image.split('.')[1], img) 

    # We can tint the image by scaling each of the color channels  
    # by a different scalar constant. The image has shape (400, 248, 3);  
    # we multiply it by the array [1, 0.95, 0.9] of shape (3,);  
    # numpy broadcasting means that this leaves the red channel unchanged,  
    # and multiplies the green and blue channels by 0.95 and 0.9  
    # respectively.  
    imgTinted = img * [1, 0.95, 0.9]  
  
    # Resize the tinted image to be 300 by 300 pixels.  
    imgTinted = imresize(imgTinted, (300, 300))  
  
    # Write the tinted image back to disk  
    imsave(image.split('.')[0]+"_tinted."+image.split('.')[1], imgTinted)

def PCA(image):
    # Read an JPEG image into a numpy array  
    evNumChosen = 50
    if './' in image:
        image = image.split('./')[1]
    img = imread(image)  
    #print img.dtype, img.shape  # Prints "uint8 (400, 248, 3)"  
    img=img.astype(np.float64)
    img -= np.mean(img,axis=0)

    imgPCA = np.zeros(img.shape)
    imgWhitening = np.zeros(img.shape)

    #one color image PCA
    cov = np.dot(img[:,:,0].T, img[:,:,0]) / img.shape[0]
    U,S,V = np.linalg.svd(cov)
    tmp_1c = np.dot(img[:,:,0],U[:,:evNumChosen])
    imgPca_1c = np.dot(tmp_1c,U.transpose()[:evNumChosen,:])
    #whitening
    tmp_1c = tmp_1c / np.sqrt(S[:evNumChosen] + 1e-5)
    imgWhitening_1c = np.dot(tmp_1c,U.transpose()[:evNumChosen,:])

    imsave(image.split('.')[0]+"_pca_1c."+image.split('.')[1], imgPca_1c)
    imsave(image.split('.')[0]+"_whitening_1c."+image.split('.')[1], imgWhitening_1c)


    #RGB image PCA
    for i in xrange(3):
        cov = np.dot(img[:,:,i].T, img[:,:,i]) / img.shape[0]
        U,S,V = np.linalg.svd(cov)
        tmp = np.dot(img[:,:,i],U[:,:evNumChosen])
        imgPCA[:,:,i] = np.dot(tmp,U.transpose()[:evNumChosen,:])
        #whitening
        tmp = tmp / np.sqrt(S[:evNumChosen] + 1e-5)
        imgWhitening[:,:,i] = np.dot(tmp,U.transpose()[:evNumChosen,:])

    
    imsave(image.split('.')[0]+"_pca."+image.split('.')[1], imgPCA)
    imsave(image.split('.')[0]+"_whitening."+image.split('.')[1], imgWhitening)
    





        
tint('./Koala.jpg')
PCA('./Koala.jpg')
