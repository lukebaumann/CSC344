package com.lukebaumann.csc344.algorithmiccomposition;


public class State {
	private int currentChord;
	private int currentOctive;
	private int currentNote;
	private int currentNoteLength;
	private int currentMeasureLengthPlayed;

	public State(int currentChord, int currentOctive, int currentNote) {
		this.currentChord = currentChord;
		this.currentOctive = currentOctive;
		this.currentNote = currentNote;
		this.currentNoteLength = 2;
		this.currentMeasureLengthPlayed = 0;
	}

	public int getCurrentChord() {
		return currentChord;
	}

	public void setCurrentChord(int currentChord) {
		this.currentChord = currentChord;
	}

	public int getCurrentOctive() {
		return currentOctive;
	}

	public void setCurrentOctive(int currentOctive) {
		this.currentOctive = currentOctive;
	}

	public int getCurrentNote() {
		return currentNote;
	}

	public void setCurrentNote(int currentNote) {
		this.currentNote = currentNote;
	}

	public int getCurrentNoteLength() {
		return currentNoteLength;
	}

	public void setCurrentNoteLength(int currentNoteLength) {
		this.currentNoteLength = currentNoteLength;
	}
	
	public int getCurrentMeasureLengthPlayed() {
		return currentMeasureLengthPlayed;
	}

	public void setCurrentMeasureLengthPlayed(int currentMeasureLengthPlayed) {
		this.currentMeasureLengthPlayed = currentMeasureLengthPlayed;
	}

}
