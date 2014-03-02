package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.Random;

public class MarkovChainMelody {
	private static final int SEED = 3;
	
	// Probabilities of the note to be played based on the current chord
	private double noteProbabilities[][] = new double[][] {
			{0.25, 0.10, 0.25, 0.10, 0.25, 0.00, 0.05},
			{0.05, 0.25, 0.10, 0.25, 0.10, 0.25, 0.00},
			{0.00, 0.05, 0.25, 0.10, 0.25, 0.10, 0.25},
			{0.25, 0.00, 0.05, 0.25, 0.10, 0.25, 0.10},
			{0.10, 0.25, 0.00, 0.05, 0.25, 0.10, 0.25},
			{0.25, 0.10, 0.25, 0.00, 0.05, 0.25, 0.10},
			{0.10, 0.25, 0.10, 0.25, 0.00, 0.05, 0.25}};
	
	// Probabilities of the next octive based on the current note
	private double moveOctiveProbabilities[][] = new double[][] {
			{0.01, 0.74, 0.25},
			{0.02, 0.83, 0.15},
			{0.03, 0.92, 0.05},
			{0.04, 0.92, 0.04},
			{0.05, 0.92, 0.03},
			{0.15, 0.83, 0.02},
			{0.25, 0.74, 0.01}};

	private Random random;
	
	public MarkovChainMelody() {
		random = new Random(SEED);
	}
	
	public void setNextStateNoteInformation(State state) {
		int currentOctive = state.getCurrentOctive();
		int currentChord = state.getCurrentChord();
		int currentNote = state.getCurrentNote();
		int changeInOctive = getChangeInOctive(currentNote);
		
		if (currentOctive + changeInOctive < 5 || currentOctive + changeInOctive > 7) {
			state.setCurrentOctive(currentOctive);
		}
		else {
			state.setCurrentOctive(currentOctive + changeInOctive);
		}
		state.setCurrentNote(getNextNote(currentChord));
	}

	private int getNextNote(int currentChord) {
		double selection = random.nextDouble();

		double sum = 0.0;
		int i = 0;
		
		for (i = 0; i < noteProbabilities.length; i++) {
			sum += noteProbabilities[currentChord][i];
			
			if (selection < sum) {
				break;
			}
		}
		
		return i;
	}

	private int getChangeInOctive(int currentNote) {
		double selection = random.nextDouble();

		double sum = 0.0;
		int i = 0;
		
		for (i = 0; i < moveOctiveProbabilities.length; i++) {
			sum += moveOctiveProbabilities[currentNote][i];
			
			if (selection < sum) {
				break;
			}
		}
		
		return i - 1;
	}
}
