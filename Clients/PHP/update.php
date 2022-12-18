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