package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.Random;

public class MarkovChainMelody {
	private static final int SEED = 4;
	private double noteLengthProbabilities[][];
	
	// Probabilities of the note to be played based on the current chord states = {I, ii, iii, IV, V, vi, vii}
	// notes = {0, 1, 2, 3, 4, 5, 6, 7}
	private double noteProbabilities[][] = new double[][] {
			{0.350, 0.050, 0.250, 0.050, 0.300, 0.000, 0.000},
			{0.000, 0.350, 0.050, 0.250, 0.050, 0.300, 0.000},
			{0.000, 0.000, 0.350, 0.050, 0.250, 0.050, 0.300},
			{0.300, 0.000, 0.000, 0.350, 0.050, 0.250, 0.050},
			{0.050, 0.300, 0.000, 0.000, 0.350, 0.050, 0.250},
			{0.250, 0.050, 0.300, 0.000, 0.000, 0.350, 0.050},
			{0.050, 0.250, 0.050, 0.300, 0.000, 0.000, 0.350}};

	// Probabilities of the note to be played based on the current chord states = {I, ii, iii, IV, V, vi, vii}
	// notes = {0, 1, 2, 3, 4, 5, 6, 7}
	private double experimentalNoteProbabilities[][] = new double[][] {
			{0.350, 0.000, 0.300, 0.000, 0.350, 0.000, 0.000},
			{0.000, 0.350, 0.000, 0.300, 0.000, 0.350, 0.000},
			{0.000, 0.000, 0.350, 0.000, 0.300, 0.000, 0.350},
			{0.350, 0.000, 0.000, 0.350, 0.000, 0.300, 0.000},
			{0.000, 0.350, 0.000, 0.000, 0.350, 0.000, 0.300},
			{0.300, 0.000, 0.350, 0.000, 0.000, 0.350, 0.000},
			{0.000, 0.300, 0.000, 0.350, 0.000, 0.000, 0.350}};
	
	// Probabilities of the length of the note to be played based on the how much of the measure has been played.
	// states =  {0, 1/8, 1/4, 3/8, 1/2, 5/8, 3/4, 7/8}
	// note lengths = {1/8, 1/4, 3/8, 1/2, 5/8, 3/4, 7/8, 1}
	private static final double normalNoteLengthProbabilities[][] = new double[][] {
			{0.30, 0.25, 0.05, 0.20, 0.05, 0.05, 0.05, 0.05},
			{0.40, 0.05, 0.30, 0.50, 0.10, 0.05, 0.05, 0.00},
			{0.20, 0.40, 0.50, 0.20, 0.50, 0.20, 0.00, 0.00},
			{0.60, 0.05, 0.20, 0.50, 0.10, 0.00, 0.00, 0.00},
			{0.35, 0.35, 0.10, 0.20, 0.00, 0.00, 0.00, 0.00},
			{0.80, 0.10, 0.10, 0.00, 0.00, 0.00, 0.00, 0.00},
			{0.50, 0.50, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
			{1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00}};
	
	// Probabilities of the length of the note to be played based on the how much of the measure has been played.
	// states =  {0, 1/8, 1/4, 3/8, 1/2, 5/8, 3/4, 7/8}
	// note lengths = {1/8, 1/4, 3/8, 1/2, 5/8, 3/4, 7/8, 1}
	private double fastNoteLengthProbabilities[][] = new double[][] {
			{0.80, 0.10, 0.05, 0.05, 0.00, 0.00, 0.00, 0.00},
			{0.80, 0.10, 0.10, 0.00, 0.00, 0.00, 0.00, 0.00},
			{0.80, 0.10, 0.10, 0.00, 0.00, 0.00, 0.00, 0.00},
			{0.80, 0.15, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00},
			{0.80, 0.15, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00},
			{0.80, 0.15, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00},
			{0.85, 0.15, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
			{1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00}};
	
	// Probabilities of the next octive based on the current note states = {0, 1, 2, 3, 4, 5, 6, 7}
	// next octive = {-1, 0, 1}
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
		int currentMeasureLengthPlayed = state.getCurrentMeasureLengthPlayed() + state.getCurrentNoteLength();
		
		if (currentMeasureLengthPlayed == 8) {
			currentMeasureLengthPlayed = 0;
		}
		state.setCurrentMeasureLengthPlayed(currentMeasureLengthPlayed);
		
		int changeInOctive = getChangeInOctive(currentNote);
		int noteLength = getNoteLength(currentMeasureLengthPlayed);
		
		if (currentOctive + changeInOctive < 6 || currentOctive + changeInOctive > 7) {
			state.setCurrentOctive(currentOctive);
		}
		else {
			state.setCurrentOctive(currentOctive + changeInOctive);
		}
		state.setCurrentNote(getNextNote(currentChord));
		state.setCurrentNoteLength(noteLength);
	}

	private int getNextNote(int currentChord) {
		double selection = random.nextDouble();

		double sum = 0.0;
		int i = 0;
		
		for (i = 0; i < experimentalNoteProbabilities.length; i++) {
			sum += experimentalNoteProbabilities[currentChord][i];
			
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
	
	private int getNoteLength(int measureLengthPlayed) {
		double selection = random.nextDouble();

		double sum = 0.0;
		int i = 0;
		
		for (i = 0; i < fastNoteLengthProbabilities.length; i++) {
			sum += fastNoteLengthProbabilities[measureLengthPlayed][i];
			
			if (selection < sum) {
				break;
			}
		}
		
		return i + 1;
	}
}
