#ifndef EVENT_H
#define EVENT_H

#include"def.h"

using namespace std;

class Track {
 public:
  Track();
  Track(stringstream &line);


 public:
  int GetTrackNum() {return trackNum;}
  int GetType() {return trackType;}
  double GetEta() {return eta;}
  double GetPhi() {return phi;}
  double GetPT() {return pt;}
  double GetJMass() {return jmass;}
  double GetNTrk() {return ntrk;}
  double GetBTag() {return btag;}
  double GetHadEM() {return hademRatio;}

 protected:
  int trackNum, trackType;
  double eta, phi, pt, jmass, ntrk, btag, hademRatio, dum1, dum2;
};

class Event {
 public:
  Event();
  Event(int num, int trig);

  Track* GetTrack(const char* trackType = "", int trackNum = 0);
  Track* GetTrack(string trackType = "", int trackNum = 0)
  {return GetTrack(trackType.c_str(), trackNum);}

  void AddTrack(Track *newTrack);

  void SortTracks();

 private:
  void SortByPT(vector<Track*> &tracks);
  Track* GrabTrack(vector<Track*> &tracks, int i);

  int eventNum, trigger;
  vector<Track*> AllTracks, Photons, Jets, Electrons, Muons, Taus,
    Exotics;
  Track *MET;
};

#endif
