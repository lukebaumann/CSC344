package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.Random;

public class MarkovChainChords {
	private static final int NUMBER_OF_CHORDS = 4;
	private int[] chords;

	private double chordIndexProbabilities[][] = new double[][] {
			{0.00, 0.12, 0.12, 0.25, 0.25, 0.25, 0.01},
			{0.00, 0.00, 0.00, 0.70, 0.30, 0.00, 0.00},
			{0.10, 0.00, 0.00, 0.70, 0.20, 0.00, 0.00},
			{0.35, 0.10, 0.00, 0.00, 0.35, 0.20, 0.00},
			{0.40, 0.00, 0.00, 0.20, 0.00, 0.40, 0.00},
			{0.20, 0.00, 0.00, 0.40, 0.40, 0.00, 0.00},
			{0.80, 0.00, 0.00, 0.00, 0.20, 0.00, 0.00}};

	private Random random;
	
	public MarkovChainChords() {
		random = new Random();
	}
	
	public MarkovChainChords(int seed) {
		random = new Random(seed);
	}
	
	public void setNextStateChordInformation(State state) {		
		int currentChord = state.getCurrentChord();
		state.setCurrentChord(getNextChord(currentChord));
	}
	
	private int getNextChord(int currentChord) {
		double selection = random.nextDouble();
		double sum = 0.0;
		int i = 0;
		
		for (i = 0; i < chordIndexProbabilities.length; i++) {
			sum += chordIndexProbabilities[currentChord][i];
			
			if (selection < sum) {
				break;
			}
		}
		
		return i;
	}
}
