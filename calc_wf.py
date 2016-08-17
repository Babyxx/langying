#from numpy import *
import sys
import operator

def getWordSet(fileName):
	fr = open(fileName)
	fileStr = fr.readlines()
	wordList = []
	for line in fileStr:
		lineStr = line.strip().split(' ')
		wordList.extend(lineStr)
	retSet = set(wordList)
	
	return retSet

def getWordCount(fileName):
	wordSet = getWordSet(fileName)
	wordCountDict = {}
	fr = open(fileName)
	fileStr = fr.readlines()
	wordSum = 0
	for word in wordSet:
		wordCountDict[word] = 0
	count = 0

	for line in fileStr:
		lineStr = line.strip().split(' ')
		lineLength = len(lineStr)
		wordSum += lineLength
		
#		print "--------"
		if(count %1000 == 0):
			sys.stderr.write("Sentences processed: %s\n" % count)
		count += 1		
		
		for i in range(lineLength):
#			if lineStr[i] not in wordFreqDict.keys():
#				wordFreqDict[lineStr[i]] = 0
			wordCountDict[lineStr[i]] += 1
#	for item in wordFreqDict.iteritems():
#		item[-1] = item[-1]/float(wordSum)

	sortedWordCountDict = sorted(wordCountDict.iteritems(), key=operator.itemgetter(1), reverse=True)
#	return wordCountDict, wordSum
	return sortedWordCountDict, wordSum



if __name__ == "__main__":
#	wordSet = getWordSet("corpus/train.zh")
#	print len(wordSet)
	
#	wordSize = len(wordSet)
#	for word in wordSet:
#		print word

	wordCountDict, wordSum = getWordCount("train/raw/2.txt")
	wordDictLength = len(wordCountDict)
#	print len(wordFreqDict)
	print wordSum
#	for item in wordCountDict.iteritems():
#		print "%s %d" %(item[0], item[1])
	for i in range(wordDictLength):
		print "%s %d" %(wordCountDict[i][0], wordCountDict[i][1])
