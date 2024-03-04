/*
  Grant Johnson
  gjohns62
  bfviewer.cpp
  Discription:
    - Takes in a decoded CSV file from a betaflight drone
    - Stores all data in frames to be recalled later
    - Finally prints out specified graphs in jgraph format
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

/* Frame to store each loopiteration */
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

  /* Make sure proper amount of variables given */
  if (argc != 3) {
    fprintf(stderr, "Usage: %s (FLAG) [log file]\n", argv[0]);
    exit(-1);
  }

  /* Open and check filestream */
  fin.open(argv[argc-1]);
  if(!fin.is_open()) {
    /* If failed to open, exit and return error */
    fprintf(stderr, "File didn't open\n");
    exit(-1);
  }

  for (i = 0; i < 44 && !fin.eof(); str = "", i++) {
    /* Get datatypes of log csv file */
    getline(fin, str, ',');
    types.push_back(str);
  }
  /* Get the final value from the row */
  str = "";
  getline(fin, str);
  types.push_back(str);

  /* Loop through the entire CSV file */
  while (!fin.eof()) {
    /* For the current line, loop through each data point */
    for (c = 0; c < 45; c++) {
      /* Get the next data point */
      if (c < 44) {
        getline(fin, str, ',');
      } else {
        getline(fin, str);
      }

      /* Parse data and save in correct field depending on position in row */
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

  /* Get the user specified flag, then check what graph to print */
  string toGraph = argv[1];
  if (toGraph == "-m") {
    /* For motors, create the graph and labels */
    printf("newgraph\n\n");
    printf("xaxis size 6\n");
    printf("label fontsize 18 : Time (s)\n");
    printf("yaxis size 3\n");
    printf("label fontsize 18 : Speed (kv)\n");
    printf("newstring hjc vjt x 25 y 1600 fontsize 20 lcolor 0 0 0 : motor[1]\n");
    printf("newcurve marktype none linetype longdash color 0 0 0 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      /* Then loop through each frame and get current time, convert to seconds */
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      /* Finally print both time in seconds and motor value */
      printf("%f %d  ", currTime, frames[i]->motors[1]);
    }
    /* Copy previous graph and translate upward */
    printf("copygraph\n\n");
    printf("y_translate 5\n");
    printf("newstring hjc vjt x 25 y 1600 fontsize 20 lcolor 0 0 0 : motor[0]\n");
    printf("\nnewcurve marktype none linetype longdash color 0 0 0 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      /* Then loop through each frame and get current time, convert to seconds */
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      /* Finally print both time in seconds and motor value */
      printf("%f %d  ", currTime, frames[i]->motors[0]);
    }
    /* For motors, create the graph and labels */
    printf("newpage\n");
    printf("newgraph\n\n");
    printf("xaxis size 6\n");
    printf("label fontsize 18 : Time (s)\n");
    printf("yaxis size 3\n");
    printf("label fontsize 18 : Speed (kv)\n");
    printf("newstring hjc vjt x 25 y 1600 fontsize 20 lcolor 0 0 0 : motor[2]\n");
    printf("\nnewcurve marktype none linetype longdash color 0 0 0 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      /* Then loop through each frame and get current time, convert to seconds */
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      /* Finally print both time in seconds and motor value */
      printf("%f %d  ", currTime, frames[i]->motors[2]);
    }
    /* Copy previous graph and translate upward */
    printf("copygraph\n\n");
    printf("y_translate -5\n");
    printf("newstring hjc vjt x 25 y 1600 fontsize 20 lcolor 0 0 0 : motor[3]\n");
    printf("\nnewcurve marktype none linetype longdash color 0 0 0 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      /* Then loop through each frame and get current time, convert to seconds */
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      /* Finally print both time in seconds and motor value */
      printf("%f %d  ", currTime, frames[i]->motors[3]);
    }
  } else if (toGraph == "-s") {
    /* For RSSI, create the graph and labels */
    printf("newgraph\n\n");
    printf("xaxis size 6\n");
    printf("label fontsize 18 : Time (s)\n");
    printf("yaxis size 3\n");
    printf("label fontsize 18 : RSSI\n");
    printf("newstring hjc vjt x 25 y 900 fontsize 20 lcolor 0 0 0 : Signal Strength\n");
    printf("newcurve marktype none linetype longdash color 0 0 0 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      /* Then loop through each frame and get current time, convert to seconds */
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      /* Finally print both time in seconds and RSSI value */
      printf("%f %d  ", currTime, frames[i]->rssi);
    }
  } else if (toGraph == "-a") {
    /* For current, create the graph and labels */
    printf("newgraph\n\n");
    printf("xaxis size 6\n");
    printf("label fontsize 18 : Time (s)\n");
    printf("yaxis size 3\n");
    printf("label fontsize 18 : Amperes (A)\n");
    printf("newstring hjc vjt x 25 y 100 fontsize 20 lcolor 0 0 0 : Current\n");
    printf("newcurve marktype none linetype longdash color 0 0 0 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      /* Then loop through each frame and get current time, convert to seconds */
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      /* Finally print both time in seconds and amperage value */
      printf("%f %f  ", currTime, frames[i]->amperage);
    }
  } else if (toGraph == "-v") {
    /* For vBat, create the graph and labels */
    printf("newgraph\n\n");
    printf("xaxis size 6\n");
    printf("label fontsize 18 : Time (s)\n");
    printf("yaxis size 3\n");
    printf("label fontsize 18 : Voltage (v)\n");
    printf("newstring hjc vjt x 25 y 16 fontsize 20 lcolor 0 0 0 : Battery Voltage\n");
    printf("newcurve marktype none linetype longdash color 0 0 0 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      /* Then loop through each frame and get current time, convert to seconds */
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      /* Finally print both time in seconds and voltage value */
      printf("%f %f  ", currTime, frames[i]->vBat);
    }
  } else if (toGraph == "-e") {
    /* For charge, create the graph and labels */
    printf("newgraph\n\n");
    printf("xaxis size 6\n");
    printf("label fontsize 18 : Time (s)\n");
    printf("yaxis size 3\n");
    printf("label fontsize 18 : Capacity (mAh)\n");
    printf("newstring hjc vjt x 25 y 200 fontsize 20 lcolor 0 0 0 : Electrical Charge Used\n");
    printf("newcurve marktype none linetype longdash color 0 0 0 pts ");
    for (i = 0; i < (int)frames.size(); i++) {
      /* Then loop through each frame and get current time, convert to seconds */
      double currTime = (frames[i]->t - frames[0]->t)/1000000.00;
      /* Finally print both time in seconds and voltage value */
      printf("%f %d  ", currTime, frames[i]->mAhTotal);
    }
  } else {
    fprintf(stderr,"Non-recognized flag\n");
  }
  printf("\n");

  /* Close the filestream and delete the frames */
  for (i = 0; i < (int)frames.size(); i++) {
    delete frames[i];
  }
  fin.close();
  return 0;
}