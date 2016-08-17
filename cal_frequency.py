#!/usr/bin/env python
import os, sys
import re
import math

def genWordGram(filename):
    file = open(filename)
    all_the_text = file.read()
    all_the_text = all_the_text.lower()
    all_the_text = re.sub("\"|,|:\'", "", all_the_text)
    #all_the_text = re.sub(".", "", all_the_text)
    add1 = []
    add2 = []
    for word in all_the_text.split():
        if(re.match('^[a-z]', word) == None):
            continue
        if(word not in uniWordGram):
            uniWordGram[word] = 1
        else:
            uniWordGram[word] = uniWordGram[word] + 1
        if(word not in add1):
            add1.append(word)

    l = len(add1)
    for i in range(l):
        if(add1[i] not in uniWordIdf):
            uniWordIdf[add1[i]] = 1
        else:
            uniWordIdf[add1[i]] = uniWordIdf[add1[i]] + 1

    num = 0
    str = ""
    for i in all_the_text.split():
        if(re.match('^[a-z]', i) == None):
            num = 0
            str = ""
            continue
        num = num + 1
        if(num == 1):
            str = i + "#"
        else:
            str = str + i
            if(str not in biWordGram):
                biWordGram[str] = 1
            else:
                biWordGram[str] = biWordGram[str] + 1
            if(str not in add2):
                add2.append(str)
            num = 1
            str = i + "#"

    l = len(add2)
    for i in range(l):
        if(add2[i] not in biWordIdf):
            biWordIdf[add2[i]] = 1
        else:
            biWordIdf[add2[i]] = biWordIdf[add2[i]] + 1

    file.close()

def genPosGram(filename):
    file = open(filename)
    all_the_text = file.read()
    all_the_text = all_the_text.lower()
    all_the_text = re.sub("\"|,|:\'", "", all_the_text)
    add1 = []
    add2 = []
    add3 = []
    for word in all_the_text.split():
        if(re.match('^[a-z]', word) == None):
            continue
        word = word[word.rfind('_'):].upper()
        if(word not in uniPosGram):
            uniPosGram[word] = 1
        else:
            uniPosGram[word] = uniPosGram[word] + 1
        if(word not in add1):
            add1.append(word)

    l = len(add1)
    for i in range(l):
        if(add1[i] not in uniPosIdf):
            uniPosIdf[add1[i]] = 1
        else:
            uniPosIdf[add1[i]] = uniPosIdf[add1[i]] + 1

    num = 0
    str = ""
    for i in all_the_text.split():
        if(re.match('^[a-z]', i) == None):
            num = 0
            str = ""
            continue
        num = num + 1
        i = i[i.rfind('_'):].upper()
        if(num == 1):
            str = i + "#"
        else:
            str = str + i
            if(str not in biPosGram):
                biPosGram[str] = 1
            else:
                biPosGram[str] = biPosGram[str] + 1
            if(str not in add2):
                add2.append(str)
            num = 1
            str = i + "#"

    l = len(add2)
    for i in range(l):
        if(add2[i] not in biPosIdf):
            biPosIdf[add2[i]] = 1
        else:
            biPosIdf[add2[i]] = biPosIdf[add2[i]] + 1

    pre = ""
    for i in all_the_text.split():
        if(re.match('^[a-z]', i) == None):
            num = 0
            str = ""
            continue
        i = i[i.rfind('_'):].upper()
        num = num + 1
        if(num == 1):
            str = i + "#"
        elif(num == 2):
            str = str + i + "#"
            pre = i
        else:
            str = str + i
            if(str not in triPosGram):
                triPosGram[str] = 1
            else:
                triPosGram[str] = triPosGram[str] + 1
            if(str not in add3):
                add3.append(str)
            num = 2
            str = pre + "#" + i + "#"

    l = len(add3)
    for i in range(l):
        if(add3[i] not in triPosIdf):
            triPosIdf[add3[i]] = 1
        else:
            triPosIdf[add3[i]] = triPosIdf[add3[i]] + 1
#str = i + "#"
    file.close()



train_word_path = "./train/raw/"
test_word_path = "./test/raw/"
train_pos_path = "./train/pos/"
test_pos_path = "./test/pos/"
uniWordGram = {}
biWordGram = {}
uniPosGram = {}
biPosGram ={}
triPosGram = {}
uniWordIdf = {}
biWordIdf = {}
uniPosIdf = {}
biPosIdf ={}
triPosIdf = {}
for filename in os.listdir(train_word_path):
    if(re.match('^\d', filename) == None):
        continue
    genWordGram(train_word_path + filename)

for filename in os.listdir(train_pos_path):
    if(re.match('^\d', filename) == None):
        continue
    genPosGram(train_pos_path + filename)

uniWordGram = sorted(uniWordGram.iteritems(), key=lambda d:d[0])
biWordGram = sorted(biWordGram.iteritems(), key=lambda d:d[0])
uniPosGram = sorted(uniPosGram.iteritems(), key=lambda d:d[0])
biPosGram = sorted(biPosGram.iteritems(), key=lambda d:d[0])
triPosGram = sorted(triPosGram.iteritems(), key=lambda d:d[0])
'''
uniWordIdf = sorted(uniWordIdf.iteritems(), key=lambda d:d[0])
biWordIdf = sorted(biWordIdf.iteritems(), key=lambda d:d[0])
uniPosIdf = sorted(uniPosIdf.iteritems(), key=lambda d:d[0])
biPosIdf = sorted(biPosIdf.iteritems(), key=lambda d:d[0])
triPosIdf = sorted(triPosIdf.iteritems(), key=lambda d:d[0])
'''
#uniWordGram[0] = (uniWordGram[0], 1)
#print(uniWordGram[0][0])
'''
for filename in os.listdir(test_word_path):
    if(re.match('^\d', filename) == None):
        continue
    genWordGram(test_word_path + filename)
for filename in os.listdir(test_pos_path):
    if(re.match('^\d', filename) == None):
        continue
    genPosGram(test_pos_path + filename)
'''

uniWordOut = open("./data/dictWordUnigram.txt", "w");
biWordOut = open("./data/dictWordBigram.txt", "w");
uniPosOut = open("./data/dictPosUnigram.txt", "w");
biPosOut = open("./data/dictPosBigram.txt", "w");
triPosOut = open("./data/dictPosTrigram.txt", "w");

uniWordOut2 = open("./data/idfWU.txt", "w");
biWordOut2 = open("./data/idfWB.txt", "w");
uniPosOut2 = open("./data/idfPU.txt", "w");
biPosOut2 = open("./data/idfPB.txt", "w");
triPosOut2 = open("./data/idfPT.txt", "w");

count = 1
for i in range(len(uniWordGram)):
    if(uniWordGram[i][1] < 3):
        continue
    uniWordOut.write("%s %d\n" % (uniWordGram[i][0], uniWordGram[i][1]))
    uniWordOut2.write("%d %d\n" % (count, uniWordIdf[uniWordGram[i][0]]))
    count = count + 1
for i in range(len(biWordGram)):
    if(biWordGram[i][1] < 3):
        continue
    biWordOut.write("%s %d\n" % (biWordGram[i][0], biWordGram[i][1]))
    biWordOut2.write("%d %d\n" % (count, biWordIdf[biWordGram[i][0]]))
    count = count + 1
for i in range(len(uniPosGram)):
    if(uniPosGram[i][1] < 3):
        continue
    uniPosOut.write("%s %d\n" % (uniPosGram[i][0], uniPosGram[i][1]))
    uniPosOut2.write("%d %d\n" % (count, uniPosIdf[uniPosGram[i][0]]))
    count = count + 1
for i in range(len(biPosGram)):
    if(biPosGram[i][1] < 3):
        continue
    biPosOut.write("%s %d\n" % (biPosGram[i][0], biPosGram[i][1]))
    biPosOut2.write("%d %d\n" % (count, biPosIdf[biPosGram[i][0]]))
    count = count + 1
for i in range(len(triPosGram)):
    if(triPosGram[i][1] < 3):
        continue
    triPosOut.write("%s %d\n" % (triPosGram[i][0], triPosGram[i][1]))
    triPosOut2.write("%d %d\n" % (count, triPosIdf[triPosGram[i][0]]))
    count = count + 1
uniWordOut.close()
biWordOut.close()
uniPosOut.close()
biPosOut.close()
triPosOut.close()

uniWordOut2.close()
biWordOut2.close()
uniPosOut2.close()
biPosOut2.close()
triPosOut2.close()
'''
for i in range(len(uniWordIdf)):
    if(uniWordIdf[i][0] in skipedUWG):
        continue
    uniWordOut.write("%d %d\n" % (count, uniWordIdf[i][1]))
    count = count + 1
for i in range(len(biWordIdf)):
    if(biWordIdf[i][1] in skipedBWG):
        continue
    biWordOut.write("%d %d\n" % (count, biWordIdf[i][1]))
    count = count + 1
for i in range(len(uniPosIdf)):
    if(uniPosIdf[i][1] in skipedUPG):
        continue
    uniPosOut.write("%d %d\n" % (count, uniPosIdf[i][1]))
    count = count + 1
for i in range(len(biPosIdf)):
    if(biPosIdf[i][1] in skipedBPG):
        continue
    biPosOut.write("%d %d\n" % (count, biPosIdf[i][1]))
    count = count + 1
for i in range(len(triPosIdf)):
    if(triPosIdf[i][1] in skipedTPG):
        continue
    triPosOut.write("%d %d\n" % (count, triPosIdf[i][1]))
    count = count + 1
'''

