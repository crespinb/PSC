package test.crespin.client;

import android.util.Log;
import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Description
 *
 * @author Catalin Prata
 *         Date: 2/12/13
 */
public class TcpClient {

    public static final String DEF_IP = "192.168.43.201"; //your computer IP address
    public static final int DEF_PORT = 2019;
    public String SERVER_IP;
    public int SERVER_PORT;
    // message to send to the server
    private String mServerMessage;
    // sends message received notifications
    private OnMessageReceived mMessageListener = null;
    // while this is true, the server will continue running
    private boolean mRun = false;
    // used to send messages
    private PrintWriter mBufferOut;
    // used to read messages from the server
    private BufferedReader mBufferIn;
    private BufferedInputStream inputStream;
    private MyAudioManager audioMgr;

    /**
     * Constructor of the class. OnMessagedReceived listens for the messages received from server
     */
    public TcpClient(OnMessageReceived listener, String ip, String port) {
        mMessageListener = listener;
        SERVER_IP = ip;
        if(ip.length() == 0)
        {
            SERVER_IP = DEF_IP;
        }
        if(port.length() != 0)
        {
            SERVER_PORT = Integer.parseInt(port);
        }
        else
        {
            SERVER_PORT = DEF_PORT;
        }
        audioMgr = new MyAudioManager();
        audioMgr.startStream();
    }

    /**
     * Sends the message entered by client to the server
     *
     * @param message text entered by client
     */
    public void sendMessage(String message) {
        if (mBufferOut != null && !mBufferOut.checkError()) {
            mBufferOut.println(message);
            mBufferOut.flush();
        }
    }

    /**
     * Close the connection and release the members
     */
    public void stopClient() {

        // send mesage that we are closing the connection
        //sendMessage(Constants.CLOSED_CONNECTION+"Kazy");

        mRun = false;

        if (mBufferOut != null) {
            mBufferOut.flush();
            mBufferOut.close();
        }

        mMessageListener = null;
        mBufferIn = null;
        mBufferOut = null;
        mServerMessage = null;
    }

    public void run() {

        mRun = true;

        try {
            //here you must put your computer's IP address.
            InetAddress serverAddr = InetAddress.getByName(SERVER_IP);

            Log.e("TCP Client", "C: Connecting...");

            //create a socket to make the connection with the server
            Socket socket = new Socket(serverAddr, SERVER_PORT);
            socket.setTcpNoDelay(true);
            socket.setReceiveBufferSize(0x800000);

            try {

                //sends the message to the server
                //mBufferOut = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);

                //receives the message which the server sends back
                mBufferIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                // send login name
                //sendMessage(Constants.LOGIN_NAME+"Kazy");

                inputStream = new BufferedInputStream(socket.getInputStream());
                //inputStream = new DataInputStream(socket.getInputStream());
                mMessageListener.messageReceived("Connection to server " + SERVER_IP + " successful!");

                //in this while the client listens for the messages sent by the server
                while (mRun) {

                    //mServerMessage = mBufferIn.readLine();
                    //Log.i("TCP Connection", "Started while loop");
                    byte[] buffer = new byte[8192*8];
                    inputStream.read(buffer, 0, 8192*8);
                    float[] audioBuffer = new float[8192*2];
                    /*for(int i = 0; i < 8192; i++)
                    {
                        //Log.i("TCP", "debug1");
                        audioBuffer[2*i] = inputStream.readFloat();
                        //Log.i("TCP", "debug2");
                        audioBuffer[2*i+1] = inputStream.readFloat();
                    }*/
                    for(int i = 0; i < 8192*2; i++)
                    {
                        byte[] temp = {buffer[4*i], buffer[4*i+1], buffer[4*i+2], buffer[4*i+3]};
                        audioBuffer[i] = ByteBuffer.wrap(temp).order(ByteOrder.LITTLE_ENDIAN).getFloat();
                    }
                    Log.i("TCP Connection", String.valueOf(audioBuffer[0]));
                    //Log.i("TCP Connection", String.format("%x", Float.floatToRawIntBits(audioBuffer[0])));
                    audioMgr.write(audioBuffer, 8192*2);

                    /*if (mServerMessage != null && mMessageListener != null) {
                        //call the method messageReceived from MyActivity class
                        mMessageListener.messageReceived(mServerMessage);
                    }*/
                    //mMessageListener.messageReceived(String.valueOf(audioBuffer[0]));

                }

                Log.e("RESPONSE FROM SERVER", "S: Received Message: '" + mServerMessage + "'");

            } catch (Exception e) {

                Log.e("TCP", "S: Error", e);

            } finally {
                //the socket must be closed. It is not possible to reconnect to this socket
                // after it is closed, which means a new socket instance has to be created.
                socket.close();
            }

        } catch (Exception e) {

            Log.e("TCP", "C: Error", e);

        }

    }

    //Declare the interface. The method messageReceived(String message) will must be implemented in the MyActivity
    //class at on asynckTask doInBackground
    public interface OnMessageReceived {
        public void messageReceived(String message);
    }
}