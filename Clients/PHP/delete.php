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