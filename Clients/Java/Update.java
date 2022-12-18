import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class Update{
    public static void main(String[] args) {
        PhoenixDB pdb = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");

        JSONObject filter = new JSONObject();
        filter.put("name", "Amit");
        filter.put("surname", "Sharma");

        JSONObject student = new JSONObject();
        student.put("name", "Amit M");
        student.put("surname", "Sharma Ji");
        student.put("age", 50);

        String response = pdb.update("students", filter.toJSONString(), student.toJSONString());
        System.out.println(response);

        pdb.close();
    }
}
