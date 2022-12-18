import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class PhoenixDB {
    Socket socket = null;
    InputStream inputStream = null;
    OutputStream outputStream = null;

    int MAX_STREAM_SIZE = 256000;

    PhoenixDB(String host, int port, String username, String password, String database){
        try {

            /**
             * Connect to Database
             * */
            socket = new Socket(host, port);

            /**
             * Initialize Input Stream.
             * */
            inputStream = socket.getInputStream();

            /**
             * Initialize Output Stream
             * */
            outputStream = socket.getOutputStream();

            /**
             * Authenticate Username and Password and Connect and get Access to Database
             * */
            String auth = "{";
            auth += "\"username\":\""+username+"\",";
            auth += "\"password\":\""+password+"\",";
            auth += "\"database\":\""+database+"\"";
            auth += "}";

            /**
             * Send Auth data from Login
             */
            outputStream.write(auth.getBytes("UTF8"));
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }

    public String add(String data, String collection) {
        byte[] response = new byte[MAX_STREAM_SIZE];
        try{
            JSONParser jsonParser = new JSONParser();
            JSONObject dataObject = (JSONObject)jsonParser.parse(data);
            JSONObject command = new JSONObject();
            command.put("operation", "add");
            command.put("collection", collection);
            command.put("data", dataObject);
            outputStream.write(command.toJSONString().getBytes(StandardCharsets.UTF_8));
            inputStream.read(response);
        }catch (IOException e){
            System.out.println(e.getMessage());
            System.out.println("IOException");
        } catch (ParseException e){
            System.out.println("Parse Exception");
            System.out.println(e.getMessage());
        }
        return new String(trim(response), StandardCharsets.UTF_8);
    }

    public String find(String collection, String filter){
        byte[] response = new byte[MAX_STREAM_SIZE];
        try{
            JSONParser jsonParser = new JSONParser();
            JSONObject filterObject = (JSONObject)jsonParser.parse(filter);
            JSONObject command = new JSONObject();
            command.put("operation", "find");
            command.put("collection", collection);
            command.put("filter", filterObject);
            outputStream.write(command.toJSONString().getBytes(StandardCharsets.UTF_8));
            inputStream.read(response);
        }catch (IOException e){
            System.out.println(e.getMessage());
            System.out.println("IOException");
        } catch (ParseException e){
            System.out.println("Parse Exception");
            System.out.println(e.getMessage());
        }
        return new String(trim(response), StandardCharsets.UTF_8);
    }

    public String delete(String collection, String filter){
        byte[] response = new byte[MAX_STREAM_SIZE];
        try{
            JSONParser jsonParser = new JSONParser();
            JSONObject filterObject = (JSONObject)jsonParser.parse(filter);
            JSONObject command = new JSONObject();
            command.put("operation", "delete");
            command.put("collection", collection);
            command.put("filter", filterObject);
            outputStream.write(command.toJSONString().getBytes(StandardCharsets.UTF_8));
            inputStream.read(response);
        }catch (IOException e){
            System.out.println(e.getMessage());
            System.out.println("IOException");
        } catch (ParseException e){
            System.out.println("Parse Exception");
            System.out.println(e.getMessage());
        }
        return new String(trim(response), StandardCharsets.UTF_8);
    }

    public String update(String collection, String filter, String data){
        byte[] response = new byte[MAX_STREAM_SIZE];
        try{
            JSONParser jsonParser = new JSONParser();
            JSONObject filterObject = (JSONObject)jsonParser.parse(filter);
            JSONObject dataObject = (JSONObject)jsonParser.parse(data);

            JSONObject command = new JSONObject();
            command.put("operation", "update");
            command.put("collection", collection);
            command.put("filter", filterObject);
            command.put("data", dataObject);

            outputStream.write(command.toJSONString().getBytes(StandardCharsets.UTF_8));
            inputStream.read(response);
        }catch (IOException e){
            System.out.println(e.getMessage());
            System.out.println("IOException");
        } catch (ParseException e){
            System.out.println("Parse Exception");
            System.out.println(e.getMessage());
        }
        return new String(trim(response), StandardCharsets.UTF_8);
    }


    public void close(){
        try{
            socket.close();
            inputStream.close();
            outputStream.close();
        }catch (Exception e){
            System.out.println(e.getMessage());
        }
    }


    private static byte[] trim(byte[] bytes) {
        int i = bytes.length - 1;
        while (i >= 0 && bytes[i] == 0){
            i--;
        }
        return Arrays.copyOf(bytes, i + 1);
    }
}
