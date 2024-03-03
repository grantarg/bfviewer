#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class Frame {
  public:
    double t;
    vector<int> p;
    vector<int> i;
    vector<int> d;
    vector<int> f;
    vector<int> rcc;
    vector<int> setpoint;
    double vBat;
    double amperage;
    int baroAlt;
    int rssi;
    vector<int> gyroADC;
    vector<int> accSmooth;
    vector<int> debug;
    vector<int> motors;
    int mAhTotal;
    string mode;
};

int main(int argc, char **argv) {
  int i, c;
  string str;
  ifstream fin;
  vector<string> types;
  vector<Frame *> frames;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s (FLAG) [log file]\n", argv[0]);
    exit(-1);
  }

  fin.open(argv[argc-1]);
  if(!fin.is_open()) {
    fprintf(stderr, "File didn't open\n");
    exit(-1);
  }

  for (i = 0; i < 44 && !fin.eof(); str = "", i++) {
    getline(fin, str, ',');
    types.push_back(str);
  }
  str = "";
  getline(fin, str);
  types.push_back(str);

  while (!fin.eof()) {
    //fprintf(stderr, "Line %d", (int)frames.size());
    for (c = 0; c < 45; c++) {
      if (c < 44) {
        getline(fin, str, ',');
        //printf("%s ", str.c_str());
      } else {
        getline(fin, str);
        //printf("%s ", str.c_str());
      }

      if (c == 1) {
        Frame *f = new Frame;
        f->t = stod(str);
        frames.push_back(f);
      } else if (c >= 2 && c < 5) {
        frames[frames.size()-1]->p.push_back(atoi(str.c_str()));
      } else if (c >= 5 && c < 8) {
        frames[frames.size()-1]->i.push_back(atoi(str.c_str()));
      } else if (c >= 8 && c < 10) {
        frames[frames.size()-1]->d.push_back(atoi(str.c_str()));
      } else if (c >= 10 && c < 13) {
        frames[frames.size()-1]->f.push_back(atoi(str.c_str()));
      } else if (c >= 13 && c < 17) {
        frames[frames.size()-1]->rcc.push_back(atoi(str.c_str()));
      } else if (c >= 17 && c < 21) {
        frames[frames.size()-1]->setpoint.push_back(atoi(str.c_str()));
      } else if (c == 21) {
        frames[frames.size()-1]->vBat = stod(str);
      } else if (c == 22) {
        frames[frames.size()-1]->amperage = stod(str);
      } else if (c == 23) {
        frames[frames.size()-1]->baroAlt = atoi(str.c_str());
      } else if (c == 24) {
        frames[frames.size()-1]->rssi = atoi(str.c_str());
      } else if (c >= 25 && c < 28) {
        frames[frames.size()-1]->gyroADC.push_back(atoi(str.c_str()));
      } else if (c >= 28 && c < 31) {
        frames[frames.size()-1]->accSmooth.push_back(atoi(str.c_str()));
      } else if (c >= 31 && c < 35) {
        frames[frames.size()-1]->debug.push_back(atoi(str.c_str()));
      } else if (c >= 35 && c < 39) {
        frames[frames.size()-1]->motors.push_back(atoi(str.c_str()));
      } else if (c == 39) {
        frames[frames.size()-1]->mAhTotal = atoi(str.c_str());
      } else if (c == 40) {
        frames[frames.size()-1]->mode = str;
      }
    }
  }

  string toGraph = argv[1];
  if (toGraph == "-m") {
    printf("newgraph\n\n");
    printf("xaxis size 8\n");
    printf("yaxis size 4\n");
    printf("newcurve marktype none linetype solid color 1 0 0 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      printf("%f %d  ", currTime, frames[i]->motors[0]);
    }
    printf("copygraph\n\n");
    printf("y_translate -5");
    printf("\nnewcurve marktype none linetype dashed color 0 1 0 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      printf("%f %d  ", currTime, frames[i]->motors[1]);
    }
    printf("newpage\n");
    printf("newgraph\n\n");
    printf("xaxis size 8\n");
    printf("yaxis size 4\n");
    printf("\nnewcurve marktype none linetype dotdotdash color 0 0 1 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      printf("%f %d  ", currTime, frames[i]->motors[2]);
    }
    printf("copygraph\n\n");
    printf("y_translate -5");
    printf("\nnewcurve marktype none linetype longdash color 1 0 1 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      printf("%f %d  ", currTime, frames[i]->motors[3]);
    }
  } else {
    printf("didn't work\n");
  }
  
  printf("\n");

  fin.close();
  return 0;
}