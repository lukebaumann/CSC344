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
public class MidiFile
{


	private static Sequence sequence;
	private static Track track;
	private static final int PPQ = 24;

	public static void main(String argv[]) {
		System.out.println("midifile begin ");
		try
		{
			sequence = new Sequence(Sequence.PPQ, PPQ);
			track = sequence.createTrack();


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

			int[] cMajor = {60, 62, 64, 65, 67, 69, 71, 72};
			for (int i = 0; i < 8; i++) {
				noteOn(cMajor[i], 0.4, PPQ * i);
				noteOff(cMajor[i], PPQ * (i + 1));
			}

			//****  set end of track (meta event) 19 ticks later  ****
			mt = new MetaMessage();
			byte[] bet = {}; // empty array
			mt.setMessage(0x2F,bet,0);
			me = new MidiEvent(mt, (long) (8 * PPQ + 10));
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
}
