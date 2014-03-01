package com.lukebaumann.csc344.algorithmiccomposition;

public class Song {
	private static Key key;
	private MarkovChainMelody melody;
	private MarkovChainChords chords;
	
	public Song(Key key) {
		melody = new MarkovChainMelody();
		chords = new MarkovChainChord();
	}
}
