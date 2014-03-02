package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.ArrayList;
import java.util.List;

public class Song {
	private static final int BASS_OCTIVE = 4;
	private static final double VELOCITY = 0.7;
	private static final int CHORD_ON_TIME = 4;
	private Key key;
	private int lengthOfSong;
	private MarkovChainMelody melody;
	private MarkovChainChords chords;
	private State state;
	private List<NoteVelocity> notesVelocities;

	
	public Song(Key key, int lengthOfSong) {
		this.key = key;
		this.lengthOfSong = lengthOfSong;
		
		state = new State(0, 6, 0);
		melody = new MarkovChainMelody();
		chords = new MarkovChainChords();
		notesVelocities = new ArrayList<NoteVelocity>();
		
		makeSong();
	}
	
	private void makeSong() {
		for (int i = 0; i < lengthOfSong; i++) {
			if (i % CHORD_ON_TIME == 0) {
				notesVelocities.addAll(getNotesVelocitiesFromChord(i, CHORD_ON_TIME));
			}

			notesVelocities.add(getNoteFromMelody(i, 1));
			
			if ((i % CHORD_ON_TIME) - 1 == 0) {
				chords.setNextStateChordInformation(state);
			}

			melody.setNextStateNoteInformation(state);
		}
	}
	
	private List<NoteVelocity> getNotesVelocitiesFromChord(int when, int howLong) {
		List<NoteVelocity> notesVelocities = new ArrayList<NoteVelocity>();
		for(Integer noteIndex : Chord.values()[state.getCurrentChord()].noteIndexes) {
			notesVelocities.add(new NoteVelocity(key.getNotes().get(noteIndex) + BASS_OCTIVE * 12, VELOCITY, when, howLong));
		}
		
		return notesVelocities;
	}
	
	private NoteVelocity getNoteFromMelody(int when, int howLong) {
		NoteVelocity noteVelocity = new NoteVelocity(key.getNotes().get(state.getCurrentNote()) + state.getCurrentOctive() * 12, VELOCITY * 1.1, when, howLong);
		return noteVelocity;
	}
	
	public List<NoteVelocity> getNoteVelocities() {
		return notesVelocities;
	}
}
