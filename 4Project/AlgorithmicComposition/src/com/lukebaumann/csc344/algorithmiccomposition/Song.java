package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.ArrayList;
import java.util.List;

public class Song {
	private static final int BASS_OCTIVE = 3;
	private static final double VELOCITY = 0.7;
	// In eighth notes
	private static final int CHORD_ON_TIME = 32;
	private static final int EIGHTH_NOTES_IN_MEASURE = 8;
	private static final int NOTES_IN_OCTIVE = 12;
	private Key key;
	// In eighth notes
	private int lengthOfSong;
	private MarkovChainMelody melody;
	private MarkovChainChords chords;
	private State state;
	private List<NoteVelocity> notesVelocities;

	
	public Song(Key key, int lengthOfSong) {
		this.key = key;
		this.lengthOfSong = lengthOfSong * 2;
		
		state = new State(0, 6, 0);
		melody = new MarkovChainMelody();
		chords = new MarkovChainChords();
		notesVelocities = new ArrayList<NoteVelocity>();
		
		makeSong();
	}
	
	private void makeSong() {
		//i is measure number
		for (int i = 0; i < lengthOfSong / 8; i++) {
			notesVelocities.addAll(getNotesVelocitiesFromChord(i, CHORD_ON_TIME));
			playMeasureWorthOfMelody(i);
			chords.setNextStateChordInformation(state);
		}
	}
	
	private List<NoteVelocity> getNotesVelocitiesFromChord(int measure, int howLong) {
		List<NoteVelocity> notesVelocities = new ArrayList<NoteVelocity>();
		for(Integer noteIndex : Chord.values()[state.getCurrentChord()].noteIndexes) {
			notesVelocities.add(new NoteVelocity(key.getNotes().get(noteIndex) + BASS_OCTIVE * NOTES_IN_OCTIVE, VELOCITY, measure * EIGHTH_NOTES_IN_MEASURE, howLong));
		}
		
		return notesVelocities;
	}
	
	// Bug here
	private void playMeasureWorthOfMelody(int measure) {
		do {
			melody.setNextStateNoteInformation(state);
			notesVelocities.add(getNoteFromMelody(measure));
		} while(state.getCurrentMeasureLengthPlayed() != 0);
	}
	
	private NoteVelocity getNoteFromMelody(int measure) {
		NoteVelocity noteVelocity = new NoteVelocity(key.getNotes().get(state.getCurrentNote()) + state.getCurrentOctive() * NOTES_IN_OCTIVE, VELOCITY * 1.1, measure * EIGHTH_NOTES_IN_MEASURE + state.getCurrentMeasureLengthPlayed(), state.getCurrentNoteLength());
		return noteVelocity;
	}
	
	public List<NoteVelocity> getNoteVelocities() {
		return notesVelocities;
	}
}
