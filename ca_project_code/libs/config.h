#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;


class config
{
private:
    string fn = "config.txt";
    string xyString, token;
    string delimiter = ",";
    ifstream configs;
    ofstream outConfigs; 
    double axisLast[2];
public:
    config();
    void getAxisLast();
    double getXaxis();
    double getYaxis();
    void setAxisLast(double x, double y);
    ~config();
};

config::config(/* args */)
{
}
void config::getAxisLast(){
    configs.open(fn);
    if(configs.is_open()){
        getline(configs, xyString);
    }
    configs.close();
    axisLast[0] = stof(xyString.substr(0, xyString.find(delimiter)));
    axisLast[0] = stof(xyString.substr(xyString.find(delimiter)+1, xyString.length()));
}
double config::getXaxis(){
    return axisLast[0];
}
double config::getYaxis(){
    return axisLast[1];
}
void config::setAxisLast(double x, double y){
    outConfigs.open(fn, ios::trunc);
    if(outConfigs.is_open()){
        outConfigs << x << "," << y << endl;
    }
}

config::~config()
{
}
