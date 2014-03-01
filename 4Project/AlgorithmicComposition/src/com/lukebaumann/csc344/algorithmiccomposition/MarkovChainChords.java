package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class MarkovChainChords {
	
	public enum Triad {		
		MAJOR(Arrays.asList(0, 4, 7)),
		MINOR(Arrays.asList(0, 3, 7)),
		DIMINISHED(Arrays.asList(0, 3, 6));
		
		List<Integer> notes;
		Triad(List<Integer> notes) {
			this.notes = notes;
		}
		
		List<Integer> transpose(int root) {
			List<Integer> ret = new ArrayList<Integer>(notes.size());
			
			for (int i = 0; i < notes.size(); i++) {
				ret.add(i, root + notes.get(i));
			}
			
			return ret;
		}
	}
	
	private static final double[][] SIMPLE_CASE = new double[][] {
			{0.1, 0.1, 0.1, 0.2, 0.3, 0.2, 0.0},
			{0.3, 0.0, 0.1, 0.1, 0.4, 0.1, 0.0},
			{0.3, 0.0, 0.0, 0.4, 0.3, 0.0, 0.0},
			{0.4, 0.0, 0.0, 0.0, 0.4, 0.2, 0.0},
			{0.4, 0.0, 0.0, 0.2, 0.0, 0.3, 0.1},
			{0.5, 0.0, 0.0, 0.2, 0.3, 0.0, 0.0},
			{0.9, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0}
		};
	
	public MarkovChainChords(int root) {
		this.root = root;
		this.random = new Random(SEED);
		this.states = getStates(root, Key.MAJOR);
	}
	
	State[] getStates(int root, Key key) {
		State[] states = new State[] {
				new State(Triad.MAJOR.transpose(key.notes[0] + root), 0.6),
				new State(Triad.MINOR.transpose(key.notes[1] + root), 0.6),
				new State(Triad.MINOR.transpose(key.notes[2] + root), 0.6),
				new State(Triad.MAJOR.transpose(key.notes[3] + root), 0.6),
				new State(Triad.MAJOR.transpose(key.notes[4] + root), 0.6),
				new State(Triad.MINOR.transpose(key.notes[5] + root), 0.6),
				new State(Triad.DIMINISHED.transpose(key.notes[6] + root), 0.6)};
		
		return states;
	}

	public State getState(int index) {
		return states[index];
	}

	public int getNumberOfStates() {
		return states.length;
	}
	
	public int getNextStateIndex(int fromState) {
		double selection = random.nextDouble();
		double sum = 0.0;
		int i = 0;
		
		for (i = 0; i < states.length; i++) {
			sum += SIMPLE_CASE[fromState][i];
			
			if (selection < sum) {
				break;
			}
		}
		
		return i;
	}
	
	public State getNextState(int fromState) {
		return states[getNextStateIndex(fromState)];
	}
}
