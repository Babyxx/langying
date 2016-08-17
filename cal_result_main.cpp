//#include "ServerSocket.h"
//#include "SocketException.h"
#include <string>
#include <iostream>
#include "svm.h"

//from origin
#include<stdlib.h>
#include<set>
#include<map>
#include<sstream>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<ctype.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FEA_NUM 83123
#define TRAIN_FILE_NUM 125

//the feature dict 
std::map<std::string,int>mapDict;
std::map<std::string,int>mapWBDict;
std::map<std::string,int>mapPUDict;
std::map<std::string,int>mapPBDict;
std::map<std::string,int>mapPTDict;

//the idf map
std::map<int,double>mapIdfWU;
std::map<int,double>mapIdfWB;
std::map<int,double>mapIdfPU;
std::map<int,double>mapIdfPB;
std::map<int,double>mapIdfPT;

//origin idf map
std::map<int,double>mapIdfWUOri;
std::map<int,double>mapIdfWBOri;
std::map<int,double>mapIdfPUOri;
std::map<int,double>mapIdfPBOri;
std::map<int,double>mapIdfPTOri;

//the feature array
std::map<int,double>mapWUVector;
std::map<int,double>mapWBVector;
std::map<int,double>mapPUVector;
std::map<int,double>mapPBVector;
std::map<int,double>mapPTVector;

//the pca stuff

double arrayAssFeature[FEA_NUM+1];
std::map<std::string, double>mapScore;
svm_model * modelGen;

void init()
{
    modelGen = svm_load_model("data/general/model");
    
    //construct the word unigram dict
    std::string strDict="data/dictWordUnigram.txt";
    std::ifstream finDict(strDict.c_str());
    std::string strDictWord;
    while(getline(finDict,strDictWord))
    {
        strDictWord=strDictWord.substr(0,strDictWord.find(" "));
        mapDict[strDictWord]=1;
    }
    finDict.close();
    int dictWordCount=0;
    for(std::map<std::string,int>::iterator iter=mapDict.begin();iter!=mapDict.end();++iter)
    {
        (*iter).second=++dictWordCount;
    }
    
    //construct word bigram index dict
    std::string strFile="data/dictWordBigram.txt";
    std::ifstream finDictWB(strFile.c_str());
    while(getline(finDictWB,strDictWord))
    {
        strDictWord=strDictWord.substr(0,strDictWord.find(" "));
        mapWBDict[strDictWord]=0;
    }
    finDictWB.close();
    for(std::map<std::string,int>::iterator iter=mapWBDict.begin();iter!=mapWBDict.end();++iter)
        (*iter).second=++dictWordCount;
    
    //construct pos unigram index dict
    strFile="data/dictPosUnigram.txt";
    std::ifstream finDictPU(strFile.c_str());
    while(getline(finDictPU,strDictWord))
    {
        strDictWord=strDictWord.substr(0,strDictWord.find(" "));
        mapPUDict[strDictWord]=0;
    }
    finDictPU.close();
    for(std::map<std::string,int>::iterator iter=mapPUDict.begin();iter!=mapPUDict.end();++iter)
        (*iter).second=++dictWordCount;
    
    //construct pos bigram index dict
    strFile="data/dictPosBigram.txt";
    std::ifstream finDictPB(strFile.c_str());
    while(getline(finDictPB,strDictWord))
    {
        strDictWord=strDictWord.substr(0,strDictWord.find(" "));
        mapPBDict[strDictWord]=0;
    }
    finDictPB.close();
    for(std::map<std::string,int>::iterator iter=mapPBDict.begin();iter!=mapPBDict.end();++iter)
        (*iter).second=++dictWordCount;
    
    //construct pos trigram index dict
    strFile="data/dictPosTrigram.txt";
    std::ifstream finDictPT(strFile.c_str());
    while(getline(finDictPT,strDictWord))
    {
        strDictWord=strDictWord.substr(0,strDictWord.find(" "));
        mapPTDict[strDictWord]=0;
    }
    finDictPT.close();
    for(std::map<std::string,int>::iterator iter=mapPTDict.begin();iter!=mapPTDict.end();++iter)
        (*iter).second=++dictWordCount;
    
    //construct the word unigram idf
    std::string strIdf;
    strFile="data/idfWU.txt";
    std::ifstream finIdfWU(strFile.c_str());
    while(getline(finIdfWU,strIdf))
    {
        int index=atoi(strIdf.substr(0,strIdf.find(" ")).c_str());
        double num=atoi(strIdf.substr(strIdf.rfind(" ")+1).c_str());
        mapIdfWU[index]=num;
    }
    finIdfWU.close();
    
    //construct the word bigram idf
    strFile="data/idfWB.txt";
    std::ifstream finIdfWB(strFile.c_str());
    while(getline(finIdfWB,strIdf))
    {
        int index=atoi(strIdf.substr(0,strIdf.find(" ")).c_str());
        double num=atoi(strIdf.substr(strIdf.rfind(" ")+1).c_str());
        mapIdfWB[index]=num;
    }
    finIdfWB.close();
    
    //construct the pos unigram idf
    strFile="data/idfPU.txt";
    std::ifstream finIdfPU(strFile.c_str());
    while(getline(finIdfPU,strIdf))
    {
        int index=atoi(strIdf.substr(0,strIdf.find(" ")).c_str());
        double num=atoi(strIdf.substr(strIdf.rfind(" ")+1).c_str());
        mapIdfPU[index]=num;
    }
    finIdfPU.close();
    
    //construct the pos bigram idf
    strFile="data/idfPB.txt";
    std::ifstream finIdfPB(strFile.c_str());
    while(getline(finIdfPB,strIdf))
    {
        int index=atoi(strIdf.substr(0,strIdf.find(" ")).c_str());
        double num=atoi(strIdf.substr(strIdf.rfind(" ")+1).c_str());
        mapIdfPB[index]=num;
    }
    finIdfPB.close();
    
    //construct the pos trigram idf
    strFile="data/idfPT.txt";
    std::ifstream finIdfPT(strFile.c_str());
    while(getline(finIdfPT,strIdf))
    {
        int index=atoi(strIdf.substr(0,strIdf.find(" ")).c_str());
        double num=atoi(strIdf.substr(strIdf.rfind(" ")+1).c_str());
        mapIdfPT[index]=num;
    }
    finIdfPT.close();
    
    mapIdfWUOri = mapIdfWU;
    mapIdfWBOri = mapIdfWB;
    mapIdfPUOri = mapIdfPU;
    mapIdfPBOri = mapIdfPB;
    mapIdfPTOri = mapIdfPT;
}

void recoverIDF()
{
    mapIdfWU = mapIdfWUOri;
    mapIdfWB = mapIdfWBOri;
    mapIdfPU = mapIdfPUOri;
    mapIdfPB = mapIdfPBOri;
    mapIdfPT = mapIdfPTOri;
}

void normalizedWordUni(std::map<int,double>&mapNor)
{
    double sum=0;
    for(std::map<int,double>::iterator iter=mapNor.begin();iter!=mapNor.end();++iter)
    {
        sum+=((*iter).second)*((*iter).second);
    }
    sum=sqrtf(sum);
    for(std::map<int,double>::iterator iter=mapNor.begin();iter!=mapNor.end();++iter)
    {
        (*iter).second/=sum;
    }
}

void pos(std::string sourceFile, std::string desFile)
{
    std::string cmd="java -mx300m -cp 'stanford-postagger.jar:' edu.stanford.nlp.tagger.maxent.MaxentTagger -model models/wsj-0-18-left3words-distsim.tagger -textFile "+ sourceFile+" > "+desFile;
    system(cmd.c_str());
}

void countTFIDF(std::map<int,double>&mapTf, std::map<int,double>&mapIdf,int filenum,int wordCount)
{
    for(std::map<int,double>::iterator iter=mapIdf.begin();iter!=mapIdf.end();++iter)
    {
        (*iter).second=log10f(filenum/(1+(*iter).second));
    }
    for(std::map<int,double>::iterator iter=mapTf.begin();iter!=mapTf.end();++iter)
    {
        (*iter).second/=(wordCount+1);
        (*iter).second*=mapIdf[(*iter).first];
    }
}
/*
void countTFIDF(std::map<int,double>&mapTf, std::map<int,double>&mapIdf,int wordCount)
{
    for(std::map<int,double>::iterator iter=mapTf.begin();iter!=mapTf.end();++iter)
    {
        if(mapIdf.find((*iter).first) == mapIdf.end())continue;
        (*iter).second/=(wordCount+1);
        (*iter).second*=mapIdf[(*iter).first];
    }
}
*/
void extractFeature(std::string strPosFile, int fileNum, svm_node* svm_node_array)
{
    mapWUVector.clear();
    mapWBVector.clear();
    mapPUVector.clear();
    mapPBVector.clear();
    mapPTVector.clear();
    
    std::string word;
    std::ifstream finWU(strPosFile.c_str());
    int wordCount=0;
    while(finWU>>word)
    {
        //fout<<word<<std::endl;
        word=word.substr(0,word.find("_"));
        for(int j=0;j<word.size();++j)
            word[j]=tolower(word[j]);
        if(!isalpha(word[0]))
            continue;
        if(mapDict.find(word)==mapDict.end())
            continue;
        ++wordCount;
        
        int wordIndex = mapDict[word];
        if(mapWUVector.find(wordIndex)==mapWUVector.end())
        {
            
            mapWUVector[wordIndex]=1;
            
            if(mapIdfWU.find(wordIndex)==mapIdfWU.end())
                mapIdfWU[wordIndex]=1;
            else
                ++mapIdfWU[wordIndex];
            
            continue;
        }
        else
            ++mapWUVector[wordIndex];
    }
    finWU.close();
    countTFIDF(mapWUVector,mapIdfWU,fileNum,wordCount);
    normalizedWordUni(mapWUVector);
    
    
    //the first feature!!
    memset(arrayAssFeature,0,sizeof(arrayAssFeature));
    arrayAssFeature[0]=wordCount;
    
    
    
    //word bigram features
    std::string preWord,postWord;
    std::ifstream finWB(strPosFile.c_str());
    while(finWB>>preWord)
    {
        preWord=preWord.substr(0,preWord.find("_"));
        for(int j=0;j<preWord.size();++j)
            preWord[j]=tolower(preWord[j]);
        if(!isalpha(preWord[0]))
            continue;
        break;
    }
    while(finWB>>word)
    {
        word=word.substr(0,word.find("_"));
        for(int j=0;j<word.size();++j)
            word[j]=tolower(word[j]);
        if(!isalpha(word[0]))
            continue;
        postWord=word;
        std::string strBigram=preWord+"#"+postWord;
        if(mapWBDict.find(strBigram)==mapWBDict.end())
        {
            preWord=postWord;
            continue;
        }
        
        preWord=postWord;
        int wordBigramIndex=mapWBDict[strBigram];
        if(mapWBVector.find(wordBigramIndex)==mapWBVector.end())
        {
            
            mapWBVector[wordBigramIndex]=1;
            
            if(mapIdfWB.find(wordBigramIndex)==mapIdfWB.end())
                mapIdfWB[wordBigramIndex]=1;
            else
                ++mapIdfWB[wordBigramIndex];
            
            continue;
        }
        else
            ++mapWBVector[wordBigramIndex];
    }
    finWB.close();
    
    wordCount=0;
    for(std::map<int,double>::iterator iter=mapWBVector.begin();iter!=mapWBVector.end();++iter)
        wordCount+=(*iter).second;
    countTFIDF(mapWBVector,mapIdfWB,fileNum,wordCount);
    normalizedWordUni(mapWBVector);
    
    //pos unigram features
    std::ifstream finPU(strPosFile.c_str());
    while(finPU>>word)
    {
        word=word.substr(word.rfind("_"));
        if(mapPUDict.find(word)==mapPUDict.end())
            continue;
        int posIndex=mapPUDict[word];
        if(mapPUVector.find(posIndex)==mapPUVector.end())
        {
            
            mapPUVector[posIndex]=1;
            
            if(mapIdfPU.find(posIndex)==mapIdfPU.end())
                mapIdfPU[posIndex]=1;
            else
                ++mapIdfPU[posIndex];
            
            continue;
        }
        else
            ++mapPUVector[posIndex];
    }
    finPU.close();
    
    wordCount=0;
    for(std::map<int,double>::iterator iter=mapPUVector.begin();iter!=mapPUVector.end();++iter)
        ++wordCount;
    countTFIDF(mapPUVector,mapIdfPU,fileNum,wordCount);
    normalizedWordUni(mapPUVector);
    
    //pos bigram features
    std::ifstream finPB(strPosFile.c_str());
    while(finPB>>preWord)
    {
        preWord=preWord.substr(preWord.rfind("_"));
        break;
    }
    while(finPB>>word)
    {
        word=word.substr(word.rfind("_"));
        postWord=word;
        //check this out !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        std::string strPosBigram=preWord+"#"+postWord;
        if(mapPBDict.find(strPosBigram)==mapPBDict.end())
        {
            preWord=word;
            continue;
        }
        int posBigramIndex=mapPBDict[strPosBigram];
        preWord=postWord;
        if(mapPBVector.find(posBigramIndex)==mapPBVector.end())
        {
            
            mapPBVector[posBigramIndex]=1;
            
            if(mapIdfPB.find(posBigramIndex)==mapIdfPB.end())
                mapIdfPB[posBigramIndex]=1;
            else
                ++mapIdfPB[posBigramIndex];
            
            continue;
        }
        else
            ++mapPBVector[posBigramIndex];
    }
    finPB.close();
    wordCount=0;
    for(std::map<int,double>::iterator iter=mapPBVector.begin();iter!=mapPBVector.end();++iter)
        ++wordCount;
    countTFIDF(mapPBVector,mapIdfPB,fileNum,wordCount);
    normalizedWordUni(mapPBVector);
    
    
    //pos trigram features
    std::ifstream finPT(strPosFile.c_str());
    std::string pre,middle,post;
    while(finPT>>pre)
    {
        pre=pre.substr(pre.rfind("_"));
        break;
    }
    while(finPT>>middle)
    {
        middle=middle.substr(middle.rfind("_"));
        break;
    }
    while(finPT>>word)
    {
        word=word.substr(word.rfind("_"));
        post=word;
        std::string strPosTrigram=pre+"#"+middle+"#"+post;
        if(mapPTDict.find(strPosTrigram)==mapPTDict.end())
        {
            pre=middle;
            middle=post;
            continue;
        }
        int posTrigramIndex=mapPTDict[strPosTrigram];
        pre=middle;
        middle=post;
        if(mapPTVector.find(posTrigramIndex)==mapPTVector.end())
        {
            
            mapPTVector[posTrigramIndex]=1;
            
            if(mapIdfPT.find(posTrigramIndex)==mapIdfPT.end())
                mapIdfPT[posTrigramIndex]=1;
            else
                ++mapIdfPT[posTrigramIndex];
            
            continue;
        }
        else
            ++mapPTVector[posTrigramIndex];
    }
    finPT.close();
    wordCount=0;
    for(std::map<int,double>::iterator iter=mapPTVector.begin();iter!=mapPTVector.end();++iter)
        ++wordCount;
    countTFIDF(mapPTVector,mapIdfPT,fileNum,wordCount);
    normalizedWordUni(mapPTVector);
    
    //do pca here lalalalala
    
    arrayAssFeature[0]/=14060;
    for(std::map<int,double>::iterator iter=mapWUVector.begin();iter!=mapWUVector.end();++iter)
        arrayAssFeature[(*iter).first]=(*iter).second;
    for(std::map<int,double>::iterator iter=mapWBVector.begin();iter!=mapWBVector.end();++iter)
        arrayAssFeature[(*iter).first]=(*iter).second;
    for(std::map<int,double>::iterator iter=mapPUVector.begin();iter!=mapPUVector.end();++iter)
        arrayAssFeature[(*iter).first]=(*iter).second;
    for(std::map<int,double>::iterator iter=mapPBVector.begin();iter!=mapPBVector.end();++iter)
        arrayAssFeature[(*iter).first]=(*iter).second;
    for(std::map<int,double>::iterator iter=mapPTVector.begin();iter!=mapPTVector.end();++iter)
        arrayAssFeature[(*iter).first]=(*iter).second;
    
    for(int i = 0;i < FEA_NUM;i++){
        svm_node_array[i].index = i+1;
        svm_node_array[i].value = arrayAssFeature[i];
    }
    svm_node_array[FEA_NUM].index = -1;
    svm_node_array[FEA_NUM].value = 0;
    
    //return svm_node_array;
} 


int main ( int argc, char * argv[] )
{
    init();
    struct dirent* ent = NULL;
    DIR *pDir;
    
    //construct the file-score map......
    std::string scoreFile = "./score.txt";
    std::ifstream finWU(scoreFile.c_str());
    std::string line;
    while(finWU>>line)
    {
        std::string name = line;
        finWU>>line;
        double score = atoi(line.c_str());
        //std::cout<<line<<"  "<<name<<"  "<<score<<std::endl;
        mapScore[name] = score;
    }
    finWU.close();
    
    std::ofstream fout;
    pDir = opendir("./test/pos");
    fout.open("./features.txt", std::ios::app);
    int i = 0;
    double fang = 0;
    while (NULL != (ent = readdir(pDir))){
        if (ent->d_type == 8){
            std::string file_name = ent->d_name;
            std::string pos_name = "test/pos/"+file_name;
            //if(file_name != "000.txt")continue;
            
            svm_node featureArray[FEA_NUM+1];
            extractFeature(pos_name, 126, featureArray);
            recoverIDF();
            for(int j = 0;j < FEA_NUM;j++){
                fout<<featureArray[j].value;
                if(j == FEA_NUM-1)fout<<"\n";
                else fout<<" ";
            }

            double value = svm_predict(modelGen, featureArray);
            
            int pos = file_name.find('.');
            file_name = file_name.substr(0, pos);
            std::cout<<file_name<<" "<<value<<std::endl;
    
            if(mapScore.find(file_name) == mapScore.end())
                continue;
            else
                fang += pow(value-mapScore[file_name], 2);
            i++;
     
            //if(i>0)break;
             
        }
    }
    std::cout<<sqrtf(fang)/(20*6)<<std::endl;
    
	return 0;
}
