import org.json.simple.JSONObject;

public class Find{
    public static void main(String[] args) {
        PhoenixDB pdb = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");

        JSONObject filter = new JSONObject();
        filter.put("name", "Amit");
        filter.put("surname", "Sharma");

        String response = pdb.find("students", filter.toJSONString());
        System.out.println(response);

        pdb.close();
    }
}
