#include"event.h"

Track::Track() {
  trackNum = trackType = -1;
}

Track::Track(stringstream &line) {
  line >> trackNum >> trackType >> eta >> phi >> pt >> jmass >> ntrk 
       >> btag >> hademRatio >> dum1 >> dum2;
}

////////////////////////////////////////////////////////////////////////////////

Event::Event() : AllTracks(0), Photons(0), Jets(0), Electrons(0),
		 Muons(0), Taus(0),  Exotics(0), MET(NULL) {
  eventNum = 0;
  trigger = 0;
}

Event::Event(int num, int trig) : AllTracks(0), Photons(0), Jets(0), 
				  Electrons(0), Muons(0), Taus(0),
				  Exotics(0), MET(NULL) {
  eventNum = num;
  trigger = trig;
}

Track* Event::GetTrack(const char* trackType, int trackNum) {
  string type = trackType;
  if(type == "Jet" || type == "jet") return GrabTrack(Jets, trackNum);
  else if(type == "Electron" || type == "electron")
    return GrabTrack(Electrons, trackNum);
  else if(type == "Muon" || type == "muon") return GrabTrack(Muons, trackNum);
  else if(type == "Tau" || type == "tau")
    return GrabTrack(Taus, trackNum);
  else if(type == "Photon" || type == "photon")
    return GrabTrack(Photons, trackNum);
  else if(type == "Exotic" || type == "exotic")
    return GrabTrack(Exotics, trackNum);
  else if(type == "MET") return MET;
  else return GrabTrack(AllTracks, trackNum);
}

void Event::AddTrack(Track *newTrack) {
  AllTracks.push_back(newTrack);
  if(newTrack -> GetType() == 0) Photons.push_back(newTrack);
  else if(newTrack -> GetType() == 1) Electrons.push_back(newTrack);
  else if(newTrack -> GetType() == 2) Muons.push_back(newTrack);
  else if(newTrack -> GetType() == 3) Taus.push_back(newTrack);
  else if(newTrack -> GetType() == 4) Jets.push_back(newTrack);
  else if(newTrack -> GetType() == 5) Exotics.push_back(newTrack);
  else if(newTrack -> GetType() == 6) MET = newTrack;
}

void Event::SortTracks() {
  SortByPT(Photons);
  SortByPT(Electrons);
  SortByPT(Muons);
  SortByPT(Taus);
  SortByPT(Jets);
  SortByPT(Exotics);
}

void Event::SortByPT(vector<Track*> &tracks) {
  Track *tempTrack;
  double max;
  for(int i = 0; i < tracks.size(); i++) {
    for(int j = i + 1; j < tracks.size(); j++) {
      if(tracks[j] -> GetPT() > tracks[i] -> GetPT()) {
	tempTrack = tracks[i];
	tracks[i] = tracks[j];
	tracks[j] = tempTrack;
      }
    }
  }
}

Track* Event::GrabTrack(vector<Track*> &tracks, int i) {
  if(i < 0 || i >= tracks.size()) return NULL;
  else return tracks[i];
}
