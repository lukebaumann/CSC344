package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.ArrayList;
import java.util.List;

import com.lukebaumann.csc344.algorithmiccomposition.MarkovChainMelody.NoteLengthProbabilities;
import com.lukebaumann.csc344.algorithmiccomposition.MarkovChainMelody.NoteProbabilities;

public class Song {
	private static final int BASS_OCTIVE = 4;
	private static final double VELOCITY = 0.7;
	// In eighth notes
	private static final int MEASURE_OF_TIME = 8;
	private static final int EIGHTH_NOTES_IN_MEASURE = 8;
	private static final int NOTES_IN_OCTIVE = 12;
	private static final int NUMBER_OF_CHORDS = 4;
	private Key key;
	// In eighth notes
	private int lengthOfSong;
	private boolean holdLastMeasure;
	private MarkovChainMelody melodyChain;
	private MarkovChainChords chordsChain;
	private int[] chords;
	private State state;
	private List<NoteVelocity> notesVelocities;

	
	public Song(Key key, int lengthOfSong, boolean holdLastMeasure) {
		if (lengthOfSong < 8) {
			throw new IllegalArgumentException("lengthOfSong needs to be at least 8");
		}

		this.key = key;
		this.lengthOfSong = lengthOfSong * 2;
		this.holdLastMeasure = holdLastMeasure;
		
		state = new State(0, 6, 0);
		melodyChain = new MarkovChainMelody(23232323);
		chordsChain = new MarkovChainChords(343434);
		
		chords = new int[NUMBER_OF_CHORDS];
		notesVelocities = new ArrayList<NoteVelocity>();

		makeSong();
	}
	
	private void setChords() {
		for (int i = 0; i < NUMBER_OF_CHORDS;i++) {
			chords[i] = state.getCurrentChord();
			chordsChain.setNextStateChordInformation(state);
		}
	}
	
	private void setMelodyToIntro() {
		melodyChain.setNoteProbabilities(NoteProbabilities.TRIAD_NOTES);
		melodyChain.setNoteLengthProbabilities(NoteLengthProbabilities.SLOW);
	}
	
	private void setMelodyToMiddle() {
		melodyChain.setNoteProbabilities(NoteProbabilities.FIRST_FIVE_NOTES);
		melodyChain.setNoteLengthProbabilities(NoteLengthProbabilities.FAST);
	}
	
	private void setMelodyToOutro() {
		melodyChain.setNoteProbabilities(NoteProbabilities.TRIAD_NOTES);
		melodyChain.setNoteLengthProbabilities(NoteLengthProbabilities.SLOW_NO_EIGTHS);
	}
	
	private void makeSong() {
		List<NoteVelocity> chordNotes = null;
		List<NoteVelocity> melodyNotes = null;
		
		int numberOfMeasures = lengthOfSong / 8;
		//i is measure number
		for (int i = 0; i < numberOfMeasures; i++) {
			if (i ==  0) {
				setChords();
				setMelodyToIntro();
			}
			if (i == 4) {
				setChords();
				setMelodyToMiddle();
			}
			else if (i == numberOfMeasures - 4) {
				setChords();
				setMelodyToOutro();
			}

			state.setCurrentChord(chords[i % NUMBER_OF_CHORDS]);
			chordNotes = getNotesVelocitiesFromChord(i);
			melodyNotes = getMeasureWorthOfMelody(i);
			
			notesVelocities.addAll(chordNotes);
			notesVelocities.addAll(melodyNotes);
		}
		
		if (holdLastMeasure) {
			NoteVelocity lastNote = melodyNotes.get(melodyNotes.size() - 1);
			lastNote.setHowLong(lastNote.getHowLong() + MEASURE_OF_TIME);
			
			for (NoteVelocity nV : chordNotes) {
				nV.setHowLong(nV.getHowLong() + MEASURE_OF_TIME);
			}
		}
	}
	
	private List<NoteVelocity> getNotesVelocitiesFromChord(int measure) {
		List<NoteVelocity> notesVelocities = new ArrayList<NoteVelocity>();
		for(Integer noteIndex : Chord.values()[state.getCurrentChord()].noteIndexes) {
			notesVelocities.add(new NoteVelocity(key.getNotes().get(noteIndex) + BASS_OCTIVE * NOTES_IN_OCTIVE, VELOCITY, measure * EIGHTH_NOTES_IN_MEASURE, MEASURE_OF_TIME));
		}
		
		return notesVelocities;
	}
	
	private List<NoteVelocity> getMeasureWorthOfMelody(int measure) {
		List<NoteVelocity> notesVelocities = new ArrayList<NoteVelocity>();
		
		while(state.getCurrentMeasureLengthPlayed() + state.getCurrentNoteLength() < 8) {
			notesVelocities.add(getNoteFromMelody(measure));
			melodyChain.setNextStateNoteInformation(state);
		}
		notesVelocities.add(getNoteFromMelody(measure));
		melodyChain.setNextStateNoteInformation(state);

		return notesVelocities;
	}
	
	private NoteVelocity getNoteFromMelody(int measure) {
		NoteVelocity noteVelocity = new NoteVelocity(key.getNotes().get(state.getCurrentNote()) + state.getCurrentOctive() * NOTES_IN_OCTIVE, VELOCITY * 1.1, measure * EIGHTH_NOTES_IN_MEASURE + state.getCurrentMeasureLengthPlayed(), state.getCurrentNoteLength());
		return noteVelocity;
	}
	
	public List<NoteVelocity> getNoteVelocities() {
		return notesVelocities;
	}
}
