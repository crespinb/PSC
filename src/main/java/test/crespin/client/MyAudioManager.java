package test.crespin.client;

import android.media.AudioAttributes;
import android.media.AudioFormat;
import android.media.AudioTrack;
import android.media.AudioManager;


public class MyAudioManager {
    int sampleRate = 44100;
    int bufferSize = AudioTrack.getMinBufferSize(sampleRate,
            AudioFormat.CHANNEL_OUT_STEREO,
            AudioFormat.ENCODING_PCM_FLOAT);
    AudioTrack mAudioTrack = new AudioTrack(
            new AudioAttributes.Builder()
                    .setUsage(AudioAttributes.USAGE_MEDIA)
                    .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                    .build(),
            new AudioFormat.Builder()
                    .setSampleRate(sampleRate)
                    .setEncoding(AudioFormat.ENCODING_PCM_FLOAT)
                    .setChannelMask(AudioFormat.CHANNEL_OUT_STEREO)
                    .build(),
            bufferSize,
            AudioTrack.MODE_STREAM,
            AudioManager.AUDIO_SESSION_ID_GENERATE);

    public void startStream()
    {
        mAudioTrack.play();
    }

    public void write(float[] buffer, int size)
    {
        mAudioTrack.write(buffer, 0, size, AudioTrack.WRITE_BLOCKING);
    }

    public void write(byte[] buffer, int size)
    {
        mAudioTrack.write(buffer, 0, size, AudioTrack.WRITE_BLOCKING);
    }
}
