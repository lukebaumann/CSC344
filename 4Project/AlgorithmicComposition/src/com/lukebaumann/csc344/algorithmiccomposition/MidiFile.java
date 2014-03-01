/**

 * midifile.java
 *
 * A very short program which builds and writes
 * a one-note Midi file.
 *
 * author  Karl Brown
 * last updated 2/24/2003
 */
package com.lukebaumann.csc344.algorithmiccomposition;

import java.io.*;
import javax.sound.midi.*; // package for all midi classes

import com.lukebaumann.csc344.algorithmiccomposition.MarkovChain.Key;
import com.lukebaumann.csc344.algorithmiccomposition.MarkovChain.Probabilities;
import com.lukebaumann.csc344.algorithmiccomposition.MarkovChain.State;
public class MidiFile
{


	private static Sequence sequence;
	private static Track track;
	private static MarkovChain chain;
	private static final int PPQ = 24;
	private static final int LENGTH_OF_SONG = 200;

	public static void main(String argv[]) {
		System.out.println("midifile begin ");
		try
		{
			sequence = new Sequence(Sequence.PPQ, PPQ);
			track = sequence.createTrack();
			chain = new MarkovChain(60, Key.HARMONIC_MINOR, Probabilities.ONE_FOUR_FIVE);

			//****  General MIDI sysex -- turn on General MIDI sound set  ****
			byte[] b = {(byte)0xF0, 0x7E, 0x7F, 0x09, 0x01, (byte)0xF7};
			SysexMessage sm = new SysexMessage();
			sm.setMessage(b, 6);
			MidiEvent me = new MidiEvent(sm,(long)0);
			track.add(me);

			//****  set tempo (meta event)  ****
			MetaMessage mt = new MetaMessage();
			byte[] bt = {0x07, (byte) 0xA1, 0x20};
			mt.setMessage(0x51, bt, 3);
			me = new MidiEvent(mt,(long)0);
			track.add(me);

			//****  set track name (meta event)  ****
			mt = new MetaMessage();
			String TrackName = new String("midifile track");
			mt.setMessage(0x03 ,TrackName.getBytes(), TrackName.length());
			me = new MidiEvent(mt,(long)0);
			track.add(me);

			//****  set omni on  ****
			ShortMessage mm = new ShortMessage();
			mm.setMessage(0xB0, 0x7D,0x00);
			me = new MidiEvent(mm,(long)0);
			track.add(me);

			//****  set poly on  ****
			mm = new ShortMessage();
			mm.setMessage(0xB0, 0x7F,0x00);
			me = new MidiEvent(mm,(long)0);
			track.add(me);

			//****  set instrument to Piano  ****
			mm = new ShortMessage();
			mm.setMessage(0xC0, 0x00, 0x00);
			me = new MidiEvent(mm,(long)0);
			track.add(me);

			makeMusic();

			//****  set end of track (meta event) 19 ticks later  ****
			mt = new MetaMessage();
			byte[] bet = {}; // empty array
			mt.setMessage(0x2F,bet,0);
			me = new MidiEvent(mt, (long) (LENGTH_OF_SONG * PPQ + 10));
			track.add(me);

			//****  write the MIDI sequence to a MIDI file  ****
			File f = new File("midifile.mid");
			MidiSystem.write(sequence,1,f);
		}
		catch(Exception e)
		{
			System.out.println("Exception caught " + e.toString());
		}
		System.out.println("midifile end ");
	}

	private static void noteOn(int note, double velocity, long when) throws InvalidMidiDataException {
		int vel = (int) (127 * velocity);
		ShortMessage mm = new ShortMessage();
		mm.setMessage(ShortMessage.NOTE_ON, note, vel);
		MidiEvent me = new MidiEvent(mm, when);
		track.add(me);
	}

	private static void noteOff(int note, long when) throws InvalidMidiDataException {
		ShortMessage mm = new ShortMessage();
		mm.setMessage(ShortMessage.NOTE_OFF, note, 0);
		MidiEvent me = new MidiEvent(mm, when);
		track.add(me);
	}
	
	private static void makeMusic() {
		int currentState = 0;
		
		for (int i = 0; i < LENGTH_OF_SONG; i++) {
			currentState = chain.getNextStateIndex(currentState);
			State s = chain.getStates()[currentState];
			try {
				noteOn(s.getNote(), s.getVelocity(), PPQ * i);
				noteOff(s.getNote(), PPQ * (i + 1));
			} catch (InvalidMidiDataException e) {
				System.out.println("Exception: " + e.toString());
				System.out.println("s.getNote(): " + s.getNote());
				System.out.println("s.getVelocity(): " + s.getVelocity());
				System.out.println("i: " + i);
				System.out.println("PPQ: " + PPQ);
				System.out.println("i * PPQ: " + (i * PPQ));
				
			}
		}
	}
}
