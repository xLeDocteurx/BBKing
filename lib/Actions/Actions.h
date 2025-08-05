#include <Defs.h>

#ifndef ACTIONS_H
#define ACTIONS_H

void updateMasterGain(State *statePointer, std::string actionParameters);
void updateCurrentMode(State *statePointer, std::string actionParameters);
void updateCurrentPartIndex(State *statePointer, std::string actionParameters);
void updateCurrentStaveIndex(State *statePointer, std::string actionParameters);
void updateCurrentOctaveIndex(State *statePointer, std::string actionParameters);
void updateCurrentSelectedStep(State *statePointer, std::string actionParameters);
void createSongAtIndex(State *statePointer, std::string actionParameters);
void deleteSongAtIndex(State *statePointer, std::string actionParameters);
void updateCurrentSongIndex(State *statePointer, std::string actionParameters);
void updateSongName(State *statePointer, std::string actionParameters);
void updateTempo(State *statePointer, std::string actionParameters);
void playFromStart(State *statePointer, std::string actionParameters);
void play(State *statePointer, std::string actionParameters);
void stop(State *statePointer, std::string actionParameters);
void soloPart(State *statePointer, std::string actionParameters);
void mutePart(State *statePointer, std::string actionParameters);
void selectPart(State *statePointer, std::string actionParameters);
void createPart(State *statePointer, std::string actionParameters);
void updateStaveNumber(State *statePointer, std::string actionParameters);
void selectStave(State *statePointer, std::string actionParameters);
void selectOctave(State *statePointer, std::string actionParameters);
void selectInstrument(State *statePointer, std::string actionParameters);
void updateSelectedInstrumentSample(State *statePointer, std::string actionParameters);
void updateInstrumentSampleVolume(State *statePointer, std::string actionParameters);
void updateInstrumentSampleStepVolume(State *statePointer, std::string actionParameters);
void updateInstrumentSamplePitch(State *statePointer, std::string actionParameters);
void updateInstrumentSampleStepPitch(State *statePointer, std::string actionParameters);
void updateInstrumentSampleStartPosition(State *statePointer, std::string actionParameters);
void updateInstrumentSampleStepStartPosition(State *statePointer, std::string actionParameters);
void updateInstrumentSampleEndPosition(State *statePointer, std::string actionParameters);
void updateInstrumentSampleStepEndPosition(State *statePointer, std::string actionParameters);
void updateInstrumentSampleIsReverse(State *statePointer, std::string actionParameters);
void updateInstrumentSampleStepIsReverse(State *statePointer, std::string actionParameters);
void toggleInstrumentStep(State *statePointer, std::string actionParameters);

#endif // ACTIONS_H
