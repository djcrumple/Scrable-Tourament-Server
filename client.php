#! /usr/bin/php

<?php


error_reporting(E_ALL);

/* Allow the script to hang around waiting for connections. */
set_time_limit(0);

/* Turn on implicit output flushing so we see what we're getting
 * as it comes in. */
ob_implicit_flush();

$serverIp = '192.168.55.33';
$serverPort = 4609;

$myIp = '192.168.55.33';
$myPort = 4610;


// Register myself.
$file = fsockopen( $serverIp, $serverPort );

$req = 
	"<request>".
		"<type>register</type>".
		"<message>".
			"<name>TEST_CLIENT</name>".
			"<ip>$myIp</ip>".
			"<port>$myPort</port>".
		"</message>".
	"</request>".
	"\n".
	"";

if( false === fwrite( $file, $req ) ) {
	echo "ERROR: Could not register.\n";
}



if (($sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
	echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
}

if (socket_bind($sock, $myIp, $myPort) === false) {
	echo "socket_bind() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}

if (socket_listen($sock, 5) === false) {
	echo "socket_listen() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}

do {
	if (($msgsock = socket_accept($sock)) === false) {
		echo "socket_accept() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
		break;
	}
	
	if (false === ($buf = socket_read($msgsock, 2048, PHP_NORMAL_READ))) {
		echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
		continue;
	}

	if (!$buf = trim($buf)) {
		echo "Empty string\n";
		continue;
	}

	try {
		$request = new SimpleXMLElement( $buf );
	} catch( Exception $e ) {
		echo $e."\n";
		continue;
	}

	echo "Recieved the following request: \n";
	print_r( $request );

	switch( $request->type ) {

	case 'ping':
		echo "I've been pinged\n"; 

		// After being pinged, I have to return 'hello' to let the server know
		// that I am here.
		$resp = "hello\n";

		socket_write( $msgsock, $resp, strlen( $resp ) );
		break;

	case 'game_state':
		$file = fsockopen( $serverIp, $serverPort );

		$req = 
			"<request>".
				"<type>play</type>".
				"<message>".
					"<move>".
						"<letter>z</letter>".
						"<x>7</x>".
						"<y>8</y>".
						"<blank>1</blank>".
					"</move>".
					"<move>".
						"<letter>a</letter>".
						"<x>8</x>".
						"<y>8</y>".
					"</move>".
					"<move>".
						"<letter>p</letter>".
						"<x>9</x>".
						"<y>8</y>".
					"</move>".
				"</message>".
			"</request>".
			"\n".
			"";

		if( false === fwrite( $file, $req ) ) {
			echo "ERROR: Could not register.\n";
		}

		break;



	default:
		echo "ERROR: unkown request type\n";
		break;
	
	}
	
	socket_close($msgsock);

} while (true);

socket_close($sock);
?>

?>
