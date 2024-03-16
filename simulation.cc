//
// Created by Clarence Provenaz on 13.03.2024.
//
#include <iostream>
#include <fstream>
#include <sstream>


#include "simulation.h"
#include "message.h"



istringstream nextLine(ifstream& file);


Simulation::Simulation(char * inputFile) {
    std::cout<< inputFile<< std::endl;
    readFile(inputFile);
    nbSim = 0 ;

}

void Simulation::readFile(char* fileName){
    ifstream file(fileName);
    if(file.fail()) exit(EXIT_FAILURE);
    readAlg(file);
    readCor(file);
    readSca(file);
}

void Simulation::readAlg(ifstream& file){
    istringstream line = nextLine(file);
    line>>nbAlg;
    for(int i=0; i<nbAlg; i++){
        line = nextLine(file);
        S2d pos;
        int age;
        line>>pos.x>>pos.y>>age;
        algs.emplace_back(pos,age);
    }
}

void Simulation::readCor(std::ifstream &file) {
    istringstream line = nextLine(file);
    line>>nbCor;
    for(int i=0; i<nbCor; i++){
        line = nextLine(file);
        S2d pos;
        int age, id, statut, dir, dev, nbSeg;
        line>>pos.x>>pos.y>>age>>id>>statut>>dir>>dev>>nbSeg;
        vector<Segment> segs;
        for(int k=0;k<nbSeg;k++){
            line = nextLine(file);
            double angle;
            double length;
            line>>angle>>length;
            if(k==0){
                segs.emplace_back(pos,angle,length);
            }else{
                S2d BasePoint = segs[k-1].getSecPoint();
                segs.emplace_back(BasePoint,angle,length);
            }
        }
        cors.emplace_back(pos,age,id,nbSeg,segs);
    }
}

void Simulation::readSca(std::ifstream &file) {
    istringstream line = nextLine(file);
    line>>nbSca;
    for(int i=0; i<nbSca; i++){
        line = nextLine(file);
        S2d pos;
        int age,radius,statut,targetId;
        line>>pos.x>>pos.y>>age>>radius>>statut>>targetId;
        scas.emplace_back(pos,age,radius,targetId);
    }
}

istringstream nextLine(ifstream& file){
    string line;
    do{
        getline(file >> ws,line);
    }while(line[0]=='#');
    istringstream lineStream(line);
    return lineStream;
}



/*
void Simulation::readFile(char* fileName){
    string line;
    ifstream file(fileName);
    if(file.fail()) exit(1);
    while(getline(file >> ws,line)){
        if(line[0]=='#') continue;
        readLine(line);
    }
}


void Simulation::readLine(string& line){
    istringstream lineStream(line);

    enum ReadStatus{NBALG,ALGS,NBCOR,COR,SEG,NBSCA,SCAS};

    static ReadStatus state(NBALG); //initial state
    static int i(0);
    //std::cout<<line<<std::endl;

    switch (state) {
        case NBALG:
            lineStream>>nbAlg;
            state = ALGS;
            break;
        case ALGS:
            if(i < nbAlg ){
                algs.push_back(readAlg(lineStream));
                i++;
            }else{
                i = 0;
                state=NBCOR;
            }
            break;
        case NBCOR:
            lineStream>>nbCor;
            state = ALGS;
            break;
        case COR:
            if(i < nbCor ){
                readCor();
                i++;
                state = SEG;
            }else{
                i = 0;
                state=NBCOR;
            }
            break;
    }
}
*/


