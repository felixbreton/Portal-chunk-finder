#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include "mingw.thread.h"
#define M_PI 3.14159265358978 //Hello !
#define ll long long
#define ull unsigned long long
#define next(N) N*=25214903917;N+=11;N%=281474976710656

using namespace std;

int getEyesFromChunkseed(ull chunkseed);
void getBestNumberOfThreadsAndSpeed();
void checkSeeds(ull baseSeed,ull nbSeedsToCheck,int stepSize=1);
void checkSeedsWithThreads(ull baseSeed,ull nbSeedsToCheck,int nbThreads=4);

int main()
{
    checkSeedsWithThreads(50000000000,50000000000,4);
    return 0;
}

void getBestNumberOfThreadsAndSpeed()
{
    int minTime(1000000000),startTime,endTime,nbThreads(1);
    bool improved(true);
    while(improved)
    {
        startTime=clock();
        checkSeedsWithThreads(0,1000000,nbThreads);
        endTime=clock();
        if(endTime-startTime<minTime)
        {
            minTime=endTime-startTime;
            cout<<nbThreads<<" thread(s) : "<<minTime<<" milliseconds taken to check 1 million seeds, speed : "<<1000000000/minTime<<" seeds/second.\n";
            nbThreads*=2;
        }
        else
        {
            improved=false;
        }
    }
}

void checkSeedsWithThreads(ull baseSeed,ull nbSeedsToCheck,int nbThreads)
{
    int iThread;
    thread myThreadArray[nbThreads];
    for(iThread=0; iThread<nbThreads; iThread++)
    {
        myThreadArray[iThread]=thread(checkSeeds,baseSeed+iThread,nbSeedsToCheck,nbThreads);
    }
    for(iThread=0; iThread<nbThreads; iThread++)
    {
        myThreadArray[iThread].join();
    }
}

void checkSeeds(ull baseSeed,ull nbSeedsToCheck,int stepSize)
{
    ull seed,RNGseed,chunkseed;
    ll var8,var10;
    int baseX,baseZ,chunkX,chunkZ,nbEyes,t(time(0));
    double angle,dist;
    for(seed=baseSeed; seed<baseSeed+nbSeedsToCheck; seed+=stepSize)
    {
        RNGseed=seed^25214903917;
        next(RNGseed);
        var8=(RNGseed>>16)<<32;
        angle=(RNGseed/140737488355328.0)*M_PI;
        next(RNGseed);
        var8+=(int)(RNGseed>>16);//Don't ask me why there is a conversion to int here, I don't know either.
        var8=var8/2*2+1;
        next(RNGseed);
        var10=(RNGseed>>16)<<32;
        dist=40+(RNGseed/8796093022208.0);
        next(RNGseed);
        var10+=(int)(RNGseed>>16);
        var10=var10/2*2+1;
        baseX=round(cos(angle)*dist);
        baseZ=round(sin(angle)*dist);
        for(chunkX=min(baseX-6,baseX+6); chunkX<=max(baseX-6,baseX+6); chunkX++)
        {
            for(chunkZ=min(baseZ-6,baseZ+6); chunkZ<=max(baseZ-6,baseZ+6); chunkZ++)
            {
                chunkseed=(var8*chunkX+var10*chunkZ)^seed;
                nbEyes=getEyesFromChunkseed(chunkseed);
                if(nbEyes>=11)
                {
                    ofstream flow("log.txt",ios::app);
                    cout<<seed<<" "<<nbEyes<<" "<<chunkX<<" "<<chunkZ<<endl;
                    flow<<seed<<" "<<nbEyes<<" "<<chunkX<<" "<<chunkZ<<endl;
                }
            }
        }
        if(seed%50000000==49999999)
        {
            cout<<seed+1<<" time : "<<time(0)-t<<endl;
        }
    }
}

int getEyesFromChunkseed(ull chunkseed)//This function is full of Azelef math magic
{
    int iEye,nbEyes(0);
    chunkseed=chunkseed^25214903917;//This is the equivalent of starting a new Java RNG
    chunkseed*=124279299069389;//This line and the one after it simulate 761 calls to next() (761 was determined by CrafterDark)
    chunkseed+=17284510777187;
    chunkseed%=281474976710656;
    if(chunkseed>253327479039590)
    {
        next(chunkseed);
        if(chunkseed>253327479039590)
        {
            nbEyes=2;
            for(iEye=2; iEye<12; iEye++) //This is the same as calling nextFloat() 10 times and comparing it to 0.9
            {
                next(chunkseed);
                if(chunkseed>253327479039590)
                {
                    nbEyes++;
                }
            }
        }
    }
    return nbEyes;
}
