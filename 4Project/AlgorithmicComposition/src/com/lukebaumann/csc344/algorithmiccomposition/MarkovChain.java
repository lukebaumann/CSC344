package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.Random;

public class MarkovChain {
	private double probabilities[][];
	private State states[];
	private int numberOfStates;
	private Random random;
	
	private static final double EPSILON = 1E-6;
	private static final int SEED = 324;
	private static final double VELOCITY = 0.7;
	private static final int MIDDLE_C = 60;
	private static final int[] C_MAJOR = {60, 62, 64, 65, 67, 69, 71, 72};

	
	public MarkovChain(int numberOfStates) {
		this.numberOfStates = numberOfStates;
		this.random = new Random(SEED);
		
		probabilities = new double[numberOfStates][numberOfStates];
		states = new State[numberOfStates];
		for (int i = 0; i < numberOfStates; i++) {
			for (int j = 0; j < numberOfStates; j++) {
				if (j == 0) {
					probabilities[i][j] = .30;
				}
				else if (j == 3) {
					probabilities[i][j] = .20;
				}
				else if (j == 4) {
					probabilities[i][j] = .25;
				}
				else if (j == 5) {
					probabilities[i][j] = .15;
				}
				else {
					probabilities[i][j] = 0.10 / (numberOfStates - 4);
				}
			}
			
			states[i] = new State(C_MAJOR[i % C_MAJOR.length], VELOCITY);
		}
	}
	
	public double[][] getProbabilities() {
		return probabilities;
	}

	public void setProbabilities(double[][] probabilities) {
		this.probabilities = probabilities;
	}

	public State[] getStates() {
		return states;
	}

	public void setStates(State[] states) {
		this.states = states;
	}

	public int getNumberOfStates() {
		return numberOfStates;
	}

	public void setNumberOfStates(int numberOfStates) {
		this.numberOfStates = numberOfStates;
	}

	public void setToStateProbabilities(int fromState, double[] newProbabilities) {
		if (fromState >= numberOfStates) {
			throw new IllegalArgumentException("fromState out of bounds. numberOfStates = " + numberOfStates + " fromState = " + fromState);
		}
		probabilities[fromState] = newProbabilities;
	}
	
	public void setProbability(int fromState, int toState, double newProbability) {
		if (fromState >= numberOfStates) {
			throw new IllegalArgumentException("fromState out of bounds. numberOfStates = " + numberOfStates + " fromState = " + fromState);
		}
		if (toState >= numberOfStates) {
			throw new IllegalArgumentException("toState out of bounds. numberOfStates = " + numberOfStates + " toState = " + toState);
		}
		
		probabilities[fromState][toState] = newProbability;
	}
	
	public int getNextStateIndex(int fromState) {
		if (fromState >= numberOfStates) {
			throw new IllegalArgumentException("fromState out of bounds. numberOfStates = " + numberOfStates + " fromState = " + fromState);
		}

		double selection = random.nextDouble();
		double sum = 0.0;
		int i = 0;
		
		for (i = 0; i < numberOfStates; i++) {
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
	
	public class State {
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
