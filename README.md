
# PhoenixDB #

PhoenixDB is a Document based database written in C++

### *Componets:* ⚙️ ###
* `Server` Server Source Code.
* `Client Programs` Sample code for Client Side.

### *Compiling Source code:* ⚡⌨️ ###
`g++ main.cpp -l wsock32 -o server`  

### *Starting Server:* 💻 ###

Server can be started simply be executing `server.exe` file which will be generated upon compilation.

Once Server is started, you are good to go.
you can connect to server using any websocket client or program 🎉.

Default configuration runs the server on `localhost:9999`  
Configuration for Server can be found and updated in `./server/app/config.json`  
User Login details can be found and updated in `./server/app/auth.json`

### *Authenticating* 🔗 ###
Once you have connected to the Database server authentication can be done simpley by sending a auth data as JSON String to server

`EG: {"username":"sudhanshu", "password":"Password@123", "database":"college"}`

JSON String should match auth details available in `./server/app/auth.json`

### *Performing CURD Operations* 📝 ###
Sample code for Performing CURD Operations is available in `./clients`

Operations can be performed by sending query to server in JSON Format 

Example JSON String to add a record to Database.
```
{
    "operation": "add",
    "collection": "college",
    "data": {
        "name": "Sudhanshu",
        "surname": "Jha",
        "age": 20,
        "city": "Indore"
    }
}
```

Example JSON String to Search in a Database
```
{
    "operation": "find",
    "collection": "college",
    "filter": {
        "name": "Sudhanshu",
        "surname": "Jha"
    }
}
```
Example JSON String to Update a record in Database.
```
{
    "operation": "update",
    "collection": "college",
    "filter": {
        "name": "Sudhanshu",
        "surname": "Jha"
    }
    "data": {
        "name": "Sudhanshu D",
        "surname": "Jha Ji",
        "collage": "IIST"
    }
}
```
Example JSON String to Delete a record from Database
```
{
    "operation": "delete",
    "collection": "college",
    "filter": {
        "name": "Sudhanshu",
        "surname": "Jha"
    }
}
```

### *Example Source Code* 👨‍💻 ###
___JAVA Example___:

Step 1: First add the PhoenixDB class to your project (File can be found in `/clients/java` folder).  
Step 2: Create Instance of PhoenixDB class, it takes 5 Arguments,

* Host Address (`localhost` in our case).
* Port Number (`9999` in our case).
* Username
* Password
* Database Name


Connecting to Database
```
public class Add {
    public static void main(String[] args) {
        PhoenixDB connection = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");
        connection.close();
    }
}
```

Adding Record to Database
```
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class Add {
    public static void main(String[] args) {
        PhoenixDB connection = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");

        JSONObject student = new JSONObject();
        student.put("name", "Amit");
        student.put("surname", "Sharma");
        student.put("age", 20);
        student.put("city", "Patna");
        JSONArray skills = new JSONArray();
        skills.add("JAVA");
        skills.add("PHP");
        student.put("skills", skills);

        String response = connection.add(student.toJSONString(), "students");
        System.out.println(response);

        connection.close();
    }
}
```

Searching Record in Database
```
import org.json.simple.JSONObject;

public class Find{
    public static void main(String[] args) {
        PhoenixDB connection = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");

        JSONObject filter = new JSONObject();
        filter.put("name", "Amit");
        filter.put("surname", "Sharma");

        String response = connection.find("students", filter.toJSONString());
        System.out.println(response);

        connection.close();
    }
}
```

Deleting Record from Database
```
import org.json.simple.JSONObject;

public class Delete{
    public static void main(String[] args) {
        PhoenixDB connection = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");

        JSONObject filter = new JSONObject();
        filter.put("name", "Amit");
        filter.put("surname", "Sharma");

        String response = connection.delete("students", filter.toJSONString());
        System.out.println(response);

        connection.close();
    }
}
```

Updating Record in Database
```
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class Update{
    public static void main(String[] args) {
        PhoenixDB connection = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");

        JSONObject filter = new JSONObject();
        filter.put("name", "Amit");
        filter.put("surname", "Sharma");

        JSONObject student = new JSONObject();
        student.put("name", "Amit M");
        student.put("surname", "Sharma Ji");
        student.put("age", 50);

        String response = connection.update("students", filter.toJSONString(), student.toJSONString());
        System.out.println(response);

        connection.close();
    }
}
```

___PHP Example___:

Step 1: First add the PhoenixDB class to your project (File can be found in `/clients/PHP` folder).  
Step 2: Create Instance of PhoenixDB class, it takes 5 Arguments,

* Host Address (`localhost` in our case).
* Port Number (`9999` in our case).
* Username
* Password
* Database Name


Connecting to Database
```
<?php
    require_once('phoenixDB.php');
    $phoenixDB = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");
    sleep(1);
?>
```

Adding Record to Database
```
<?php

    print("PhoenixDB<br>");

    require_once('phoenixDB.php');
    $phoenixDB = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");
    sleep(1);

    $names = ["Sudhanshu", "Saurabh", "Sparsh", "Rahul"];
    $surnames = ["Jha", "Yadav", "Rathi", "Joshi"];
    $ages = [20, 22, 23, 21];

    for($i=0; $i<sizeof($names); $i++){
        $data = json_encode(
            array(
                'name' => $names[$i],
                'surname' => $surnames[$i],
                'age' => $ages[$i],
                'city' => 'Indore',
                'skills' => ['PHP','C++','JAVA']
            )
        );
        $response = $phoenixDB->add("students", $data);
        print('<pre>');
        print_r($response);
        print('</pre>');
    }

?>
```

Searching Record in Database
```
<?php

    print("PhoenixDB<br>");

    require_once('phoenixDB.php');
    $phoenixDB = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");
    sleep(1);

    $filter = json_encode(
        array(
            'name' => 'Sudhanshu',
            'surname' => 'Jha'
        )
    );
    $response = $phoenixDB->find("students", null);
    print('<pre>');
    print_r($response);
    print('</pre>');

?>
```

Deleting Record from Database
```
<?php

    print("PhoenixDB<br>");

    require_once('phoenixDB.php');
    $phoenixDB = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");
    sleep(1);

    $filter = json_encode(
        array(
            'name' => 'Sudhanshu',
            'surname' => 'Jha'
        )
    );
    $response = $phoenixDB->delete("students", $filter);
    print('<pre>');
    print_r($response);
    print('</pre>');

?>
```

Updating Record in Database
```
<?php

    print("PhoenixDB<br>");

    require_once('phoenixDB.php');
    $phoenixDB = new PhoenixDB("localhost", 9999, "sudhanshu", "Password@123", "college");
    sleep(1);

    $data = json_encode(
        array(
            'name' => 'Sudhanshu D',
            'surname' => 'Jha Ji',
            'collage' => 'IIST'
        )
    );

    $filter = json_encode(
        array(
            'name' => 'Sudhanshu',
            'surname' => 'Jha',
            'age' => 24
        )
    );
    $response = $phoenixDB->update("students", $filter, $data);
    print('<pre>');
    print_r($response);
    print('</pre>');

?>
```
***