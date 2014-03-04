package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.ArrayList;
import java.util.List;

public class Song {
	private static final int BASS_OCTIVE = 4;
	private static final double VELOCITY = 0.7;
	// In eighth notes
	private static final int CHORD_ON_TIME = 8;
	private static final int EIGHTH_NOTES_IN_MEASURE = 8;
	private static final int NOTES_IN_OCTIVE = 12;
	private static final int NUMBER_OF_CHORDS = 4;
	private Key key;
	// In eighth notes
	private int lengthOfSong;
	private MarkovChainMelody melodyChain;
	private MarkovChainChords chordsChain;
	private int[] chords;
	private State state;
	private List<NoteVelocity> notesVelocities;

	
	public Song(Key key, int lengthOfSong) {
		this.key = key;
		this.lengthOfSong = lengthOfSong * 2;
		
		state = new State(0, 6, 0);
		melodyChain = new MarkovChainMelody();
		chordsChain = new MarkovChainChords();
		
		chords = new int[NUMBER_OF_CHORDS];
		for (int i = 0; i < NUMBER_OF_CHORDS;i++) {
			chords[i] = state.getCurrentChord();
			chordsChain.setNextStateChordInformation(state);
		}
		
		notesVelocities = new ArrayList<NoteVelocity>();
		
		makeSong();
	}
	
	private void makeSong() {
		//i is measure number
		for (int i = 0; i < lengthOfSong / 8; i++) {
			state.setCurrentChord(chords[i % NUMBER_OF_CHORDS]);
			notesVelocities.addAll(getNotesVelocitiesFromChord(i));
			playMeasureWorthOfMelody(i);
		}
	}
	
	private List<NoteVelocity> getNotesVelocitiesFromChord(int measure) {
		List<NoteVelocity> notesVelocities = new ArrayList<NoteVelocity>();
		for(Integer noteIndex : Chord.values()[state.getCurrentChord()].noteIndexes) {
			notesVelocities.add(new NoteVelocity(key.getNotes().get(noteIndex) + BASS_OCTIVE * NOTES_IN_OCTIVE, VELOCITY, measure * EIGHTH_NOTES_IN_MEASURE, CHORD_ON_TIME));
		}
		
		return notesVelocities;
	}
	
	private void playMeasureWorthOfMelody(int measure) {
		while(state.getCurrentMeasureLengthPlayed() + state.getCurrentNoteLength() < 8) {
			notesVelocities.add(getNoteFromMelody(measure));
			melodyChain.setNextStateNoteInformation(state);
		}
		notesVelocities.add(getNoteFromMelody(measure));
		melodyChain.setNextStateNoteInformation(state);
	}
	
	private NoteVelocity getNoteFromMelody(int measure) {
		NoteVelocity noteVelocity = new NoteVelocity(key.getNotes().get(state.getCurrentNote()) + state.getCurrentOctive() * NOTES_IN_OCTIVE, VELOCITY * 1.1, measure * EIGHTH_NOTES_IN_MEASURE + state.getCurrentMeasureLengthPlayed(), state.getCurrentNoteLength());
		return noteVelocity;
	}
	
	public List<NoteVelocity> getNoteVelocities() {
		return notesVelocities;
	}
}
