package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.Random;

public class MarkovChainMelody {
	int currentOctive;
	int currentNoteIndex;
	int currentChordIndex;
	State currentState;
	// Probabilities of the note to be played based on the current chord
	private double noteIndexProbabilities[][] = new double[][] {
			{0.25, 0.10, 0.25, 0.10, 0.25, 0.00, 0.05},
			{0.25, 0.00, 0.05, 0.25, 0.10, 0.25, 0.10},
			{0.10, 0.25, 0.00, 0.05, 0.25, 0.10, 0.25},
			{0.25, 0.10, 0.25, 0.00, 0.05, 0.025, 0.10}};
	
	private double moveOctiveProbabilities[][] = new double[][] {
			{0.01, 0.54, 0.45},
			{0.02, 0.68, 0.30},
			{0.05, 0.85, 0.10},
			{0.07, 0.86, 0.07},
			{0.10, 0.85, 0.05},
			{0.30, 0.68, 0.02},
			{0.01, 0.54, 0.45}};

	private Random random;
	
	public MarkovChainMelody() {
		currentOctive = 6;
		currentNoteIndex = 0;
		currentChordIndex = 0;;
	}
	
	public State getNextStateIndex(State fromState) {
		State toState = new State(getNextOctive(fromState), getNextNoteIndex(fromState));
		return toState;
	}

	private int getNextNoteIndex(State fromState) {
		double selection = random.nextDouble();

		double sum = 0.0;
		int i = 0;
		
		for (i = 0; i < noteIndexProbabilities.length; i++) {
			sum += noteIndexProbabilities[fromState.getCurrentChord()][i];
			
			if (selection < sum) {
				break;
			}
		}
		
		return i;
	}

	private int getNextOctive(State fromState) {
		double selection = random.nextDouble();

		double sum = 0.0;
		int i = 0;
		
		for (i = 0; i < moveOctiveProbabilities.length; i++) {
			sum += moveOctiveProbabilities[fromState.getCurrentNote()][i];
			
			if (selection < sum) {
				break;
			}
		}
		
		return fromState.getCurrentOctive() + i - 1;
	}
}
