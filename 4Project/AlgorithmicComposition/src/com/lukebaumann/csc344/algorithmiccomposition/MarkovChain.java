package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.Random;

public class MarkovChain {
	private double probabilities[][];
	private State states[];
	private int root;
	private Key key;
	private Random random;
	
	private static final double EPSILON = 1E-6;
	private static final int SEED = 324;
	private static final double VELOCITY = 0.7;
	private static final int MIDDLE_C = 60;
	
	public enum Key {		
		MAJOR(new int[] {0, 2, 4, 5, 7, 9, 11}),
		MINOR(new int[] {0, 2, 3, 5, 6, 8, 10}),
		HARMONIC_MINOR(new int[] {0, 2, 3, 5, 6, 8, 11});
		
		int[] notes;
		Key(int[] notes) {
			this.notes = notes;
		}
		
		State[] getStates(int root) {
			State[] states = new State[notes.length];
			for (int i = 0; i < notes.length; i++) {
				states[i] = new State(root + notes[i], VELOCITY);
			}
			
			return states;
		}
	}
	
	public enum Probabilities {
		ONE_FOUR_FIVE(new double[][] {
			{0.35, 0.025, 0.025, 0.25, 0.030, 0.025, 0.025,},
			{0.35, 0.025, 0.025, 0.25, 0.030, 0.025, 0.025},
			{0.35, 0.025, 0.025, 0.25, 0.030, 0.025, 0.025},
			{0.35, 0.025, 0.025, 0.25, 0.030, 0.025, 0.025},
			{0.35, 0.025, 0.025, 0.25, 0.030, 0.025, 0.025},
			{0.35, 0.025, 0.025, 0.25, 0.030, 0.025, 0.025},
			{0.35, 0.025, 0.025, 0.25, 0.030, 0.025, 0.025}
		});
		
		double[][] probabilities;
		
		Probabilities(double[][] probabilities) {
			this.probabilities = probabilities;
		}
	}
	
	public MarkovChain(int root, Key key, Probabilities probabilities) {
		this.root = root;
		this.key = key;
		this.random = new Random(SEED);
		this.states = key.getStates(root);
		this.probabilities = probabilities.probabilities;
	}
	
	public double[][] getProbabilities() {
		return probabilities;
	}

	public void setProbabilities(Probabilities probabilities) {
		this.probabilities = probabilities.probabilities;
	}

	public State[] getStates() {
		return states;
	}

	public void setStates(State[] states) {
		this.states = states;
	}

	public int getNumberOfStates() {
		return states.length;
	}

	public void setToStateProbabilities(int fromState, double[] newProbabilities) {
		probabilities[fromState] = newProbabilities;
	}
	
	public void setProbability(int fromState, int toState, double newProbability) {
		probabilities[fromState][toState] = newProbability;
	}
	
	public int getNextStateIndex(int fromState) {
		double selection = random.nextDouble();
		double sum = 0.0;
		int i = 0;
		
		for (i = 0; i < states.length; i++) {
			sum += probabilities[fromState][i];
			
			if (selection < sum) {
				break;
			}
		}
		
		return i;
	}
	
	public State getNextState(int fromState) {
		return states[getNextStateIndex(fromState)];
	}
	
	static class State {
		int note = 0;
		double velocity = 0;
		
		public State(int note, double velocity) {
			this.note = note;
			this.velocity = velocity;
		}
		
		public int getNote() {
			return note;
		}
		
		public void setNote(int note) {
			this.note = note;
		}
		
		public double getVelocity() {
			return velocity;
		}
		
		public void setVelocity(double velocity) {
			this.velocity = velocity;
		}
	}
}
