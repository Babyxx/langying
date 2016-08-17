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

#define FEA_NUM 258574
#define TRAIN_FILE_NUM 126
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

//the feature array 258574
std::map<int,double>mapWUVector;
std::map<int,double>mapWBVector;
std::map<int,double>mapPUVector;
std::map<int,double>mapPBVector;
std::map<int,double>mapPTVector;

//the pca stuff

double arrayAssFeature[FEA_NUM+1];
std::map<std::string, double>mapScore;

void pos(std::string sourceFile, std::string desFile)
{
    std::string cmd="java -mx300m -cp 'stanford-postagger.jar:' edu.stanford.nlp.tagger.maxent.MaxentTagger -model models/wsj-0-18-left3words-distsim.tagger -textFile "+ sourceFile+" > "+desFile;
    system(cmd.c_str());
}


int main ( int argc, char * argv[] )
{

            pos("./dog.txt", "./dog_pos.txt");
            //std::cout<<ent->d_name<<std::endl;
    
	return 0;
}
