To build and run:
make
fft-[system specific name] wavefile.wav

To test:
I have included sample wave files with file names that correspond to the frequency of the tone produced in them.
30.wav, 200.wav, 500.wav, 1000.wav, 9832.wav, 200-500-1000-9832-30.wav, and whistle.wav

Demonstration:
http://youtu.be/G9GwKFU43hY

Explanation:
This is my own implementation of the FFT. The algorithm is based on the pseudocode found on wikipedia's FFT page.

I tried to create a graphical representation of the FFT output because I saw that it was difficult to prove to the class that my program was doing what I said it was doing without some kind of visualization. While designing and testing the visualization, I needed to redesign my FFT program into a state machine that returned the FFT of the next frame instead of computing all the frames at once. While working on the visualization, I found several bugs in my FFT program that may or may not have been introduced during the state machine change.

The visualizer is written with the SFML library. There is a bar for different discrete frequencies. I limit the bottom frequency to be the one right above 0 Hz and the top frequency is based on the frequency difference between bins * 780 because I have 780 pixels and each bin is one pixel wide. If I show all the frequencies, the second half is a mirror of the first half. This gives me confidence that my code was working.

It would be nice to have the music playing along the FFT visualization but that is well beyond the scope of this project. For the project I tried to make the window delta a small enough size so that it takes about as long as the length of the sound to do all the ffts

It would be nice to somehow logarithmically scale the frequencies so there is more resolution near the lower frequencies but I am not sure how to do this correctly.
