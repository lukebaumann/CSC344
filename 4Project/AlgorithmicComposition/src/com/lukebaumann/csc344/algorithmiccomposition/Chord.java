package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.Arrays;
import java.util.List;

public enum Chord {
	I(Arrays.asList(0, 2, 4)),
	ii(Arrays.asList(1, 3, 5)),
	iii(Arrays.asList(2, 4, 6)),
	IV(Arrays.asList(3, 5, 7)),
	V(Arrays.asList(4, 6, 8)),
	vi(Arrays.asList(5, 7, 9)),
	vii(Arrays.asList(6, 8, 10));
	
	List<Integer> noteIndexes;
	
	Chord(List<Integer> noteIndexes) {
		this.noteIndexes = noteIndexes;
	}
	
/*
 	List<Integer> invert(int inversion) {
		List<Integer> ret = new ArrayList<Integer>();
		if (inversion == 1) {
			ret.add(0, noteIndexes.get(0) + 0);
			ret.add(1, noteIndexes.get(1) + 0);
			ret.add(2, noteIndexes.get(2) + 0);
		}
		
		else if (inversion == 2) {
			ret.add(0, noteIndexes.get(0) + 2);
			ret.add(1, noteIndexes.get(1) + 2);
			ret.add(2, noteIndexes.get(2) + 3);
		}
		
		else if (inversion == 3) {
			ret.add(0, noteIndexes.get(0) + 4);
			ret.add(1, noteIndexes.get(1) + 5);
			ret.add(2, noteIndexes.get(2) + 5);
		}
		
		else {
			throw new IllegalArgumentException("Inversion must be between 1 and 3");
		}
		
		return ret;
	}
*/
}
