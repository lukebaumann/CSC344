package com.lukebaumann.csc344.algorithmiccomposition;

public class NoteVelocity {
	int note;
	double velocity;
	int when;
	int howLong;

	public NoteVelocity(int note, double velocity, int when, int howLong) {
		super();
		this.note = note;
		this.velocity = velocity;
		this.when = when;
		this.howLong = howLong;
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
	
	public int getWhen() {
		return when;
	}

	public void setWhen(int when) {
		this.when = when;
	}

	public int getHowLong() {
		return howLong;
	}

	public void setHowLong(int howLong) {
		this.howLong = howLong;
	}
}
