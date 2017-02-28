import cv2
import getopt
import sys

if __name__ == '__main__':
    
    args,residual = getopt.getopt(sys.argv[1:], '', ['input1=', 'input2='])
    try:
        residual = residual[0]
    except:
        residual = 0
    print residual
    print args
    args = dict(args)
    print args
    #print args.get("--input1","default input1")
    #print args.get("--input2","default input2")
    #print args.get("--input3","default input3")
    
    
    img = cv2.imread("..\..\..\git\opencv\samples\data\lena.jpg")
    #print img
    cv2.imshow("lena",img)
    #cv2.waitKey(10000)