<?php

/**
 * First enable SOCKETS in php.ini file
 */

class PhoenixDB{
    /**
     * Set MAX_MESSAGE_LENGTH to accomodate upto 1MB of Data.
     */
    const MAX_MESSAGE_LENGTH = 1048576;
    private $socket;
    function __construct($host, $port, $username, $password, $database){
        
        /**
         * Prepare JSON String for authentication
         */
        $authData = json_encode(
            array(
                'username' => $username,
                'password' => $password,
                'database' => $database
            )
        );

        /**
         * Step 1: Create Socket
         * Step 2: Connect to Server
         * Step 3: Authenticate
         */
        $this->socket = socket_create(AF_INET, SOCK_STREAM, 0) or die("Failed to create Socket.");
        socket_connect($this->socket, $host, $port) or die("Failed to connect to $host:$port");
        socket_write($this->socket, $authData, strlen($authData));
    }

    function add($collection, $data){
        $query = json_encode(
            array(
                'operation' => 'add',
                'collection' => $collection,
                'data' => json_decode($data)
            )
        );
        socket_write($this->socket, $query, strlen($query));
        return socket_read($this->socket, self::MAX_MESSAGE_LENGTH);
    }

    function update($collection, $filter, $data){
        $query = json_encode(
            array(
                'operation' => 'update',
                'collection' => $collection,
                'filter' => json_decode($filter),
                'data' => json_decode($data)
            )
        );
        socket_write($this->socket, $query, strlen($query));
        return socket_read($this->socket, self::MAX_MESSAGE_LENGTH);
    }

    function find($collection, $filter){
        $query = json_encode(
            array(
                'operation' => 'find',
                'collection' => $collection,
                'filter' => json_decode($filter)
            )
        );
        socket_write($this->socket, $query, strlen($query));
        return socket_read($this->socket, self::MAX_MESSAGE_LENGTH);
    }

    function delete($collection, $filter){
        $query = json_encode(
            array(
                'operation' => 'delete',
                'collection' => $collection,
                'filter' => json_decode($filter)
            )
        );
        socket_write($this->socket, $query, strlen($query));
        return socket_read($this->socket, self::MAX_MESSAGE_LENGTH);
    }

    function __destruct() {
        socket_close($this->socket);
    }


}

?>