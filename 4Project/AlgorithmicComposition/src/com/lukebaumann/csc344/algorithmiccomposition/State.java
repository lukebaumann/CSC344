package com.lukebaumann.csc344.algorithmiccomposition;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class State {
	private List<NoteVelocity> notesVelocities;
	
	public State(int note, double velocity) {
		this.notesVelocities = Arrays.asList(new NoteVelocity(note, velocity));
	}
	
	public State(List<Integer> notes, double velocity) {
		this.notesVelocities = new ArrayList<NoteVelocity>();
		for (int i = 0; i < notes.size(); i++) {
			notesVelocities.add(i, new NoteVelocity(notes.get(i), velocity));
		}
	}
	
	public State(List<NoteVelocity> notesVelocities) {
		this.notesVelocities = notesVelocities;
	}

	public List<NoteVelocity> getNotesVelocities() {
		return notesVelocities;
	}

	public int getNote(int index) {
		return notesVelocities.get(index).getNote();
	}

	public void setNote(int index, int note) {
		notesVelocities.get(index).setNote(note);
	}

	public double getVelocity(int index) {
		return notesVelocities.get(index).getVelocity();
	}

	public void setVelocity(int index, double velocity) {
		notesVelocities.get(index).setVelocity(velocity);
	}
	
	public class NoteVelocity {
		int note;
		double velocity;

		public NoteVelocity(int note, double velocity) {
			super();
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
