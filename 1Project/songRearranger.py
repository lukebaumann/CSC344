#!/usr/local/bin/python

import sys
import wave
import struct
import array
import random
import itertools

# http://stackoverflow.com/questions/2063284
# With changes
def wavLoad (fname):
   wav = wave.open (fname, "r")
   params = wav.getparams();
   (nchannels, sampwidth, framerate, nframes, comptype, compname) = params
   frames = wav.readframes(nframes * nchannels)
   out = struct.unpack_from("%dh" %nframes * nchannels, frames)

   frames = list(out)

   wav.close()
  
   return params, frames 

# http://soledadpenades.com/2009/10/29/
# With changes
def shuffle(fname, params, frames):
   (nchannels, sampwidth, framerate, nframes, comptype, compname) = params

   wav = wave.open(fname, "w")
   wav.setparams((nchannels, sampwidth, framerate, 0, comptype, compname))

   outFrames = []
   realTimePosition = 0 
   laterTimePosition = int(framerate * 90)
   repeatsPerClip = [16, 1, 8, 1]
#   secondsPerClip = [1, 1, 1, 1, 1, 1, 1, 1, .5, .5, .5, .5, 2, .1, .1, .1, .1, .1, .1, .1, .1, .1, .1, 1, 1, .5, .5, 1, 1, 1, 1, .5, .5, .5, .5, .5, .5, .5, .5, 2, 2, .25, .25, .25, .25, .25, .25, .25, .25]
   secondsPerClip = [2.45, 3.5, .4, 2.85, 1.15, 6.0, 1.41, 2.25, 1.84, 2.86, .78, 2.26, 1.34, 3.19]
   for i in range(len(secondsPerClip)):
      print 'i = ' + str(i)
      repeats = repeatsPerClip[i % len(repeatsPerClip)]
      print 'repeats = ' + str(repeats)
      seconds = nchannels * secondsPerClip[i % len(secondsPerClip)]
      print 'seconds = ' + str(seconds)
      length = (int) (framerate / repeats * seconds)
      print 'length = ' + str(length)

      if i == 0:
         position = realTimePosition
      elif i % 2 == 0:
         position = laterTimePosition
      else:
         position = realTimePosition
#         position = random.randint(0, nframes - length)

      realTimePosition += (int)(framerate * seconds) 
      laterTimePosition += (int)(framerate * seconds) 

      print 'position = ' + str(position)

      temp = []
      for i in range(repeats):
         temp.append(frames[position:position+length])

      temp = list(itertools.chain.from_iterable(temp))

      print 'len(temp) = ' + str(len(temp))
      packed = struct.pack("%dh" % len(temp), *temp)

      outFrames.append(packed)

   packed = ''.join(outFrames)

   wav.writeframes(packed)
   wav.close()


if len(sys.argv) < 3:
   print 'usage: songRearranger.py input.wav output.wav'
else:
   params, frames = wavLoad(sys.argv[1])
   shuffle(sys.argv[2], params, frames)

