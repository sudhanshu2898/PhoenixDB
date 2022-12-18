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