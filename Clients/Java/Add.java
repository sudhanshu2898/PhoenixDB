import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class Add {
    public static void main(String[] args) {
        PhoenixDB pdb = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");

        JSONObject student = new JSONObject();
        student.put("name", "Amit");
        student.put("surname", "Sharma");
        student.put("age", 20);
        student.put("city", "Patna");
        JSONArray skills = new JSONArray();
        skills.add("JAVA");
        skills.add("PHP");
        student.put("skills", skills);

        String response = pdb.add(student.toJSONString(), "students");
        System.out.println(response);

        pdb.close();
    }
}
