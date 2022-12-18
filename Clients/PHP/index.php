<html>
    <head>
        <title>PheonixDB</title>
        <style>
            a{
                text-decoration: none;
            }
            table{
                border: 1px solid black;
            }
            td{
                border: 1px solid black;
                padding: 5px 25px;
            }
        </style>
    </head>
    <body>
        <table>
        <?php
            if ($handle = opendir('.')) {
                $count = 1;
                while (false !== ($entry = readdir($handle))) {
                    if ($entry != "." && $entry != ".." && is_file($entry)) {
                        print('<tr>');
                        print('<td>'.$count.'</td>');
                        print('<td><a href="'.$entry.'">'.$entry.'</a></td>');
                        print('</tr>');
                        $count++;
                    }
                }
                closedir($handle);
            }
        ?>
        </table>
    </body>
</html>