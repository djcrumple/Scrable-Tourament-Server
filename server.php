#! /usr/bin/php

<?php

// List of all registered players indexed by their name. For each player
// we will know their IP and Port number.
$playerList = array();

function register_player( $name, $ip, $port ) {
	$player = (object)array(
		'name' => $name,
		'ip' => $ip,
		'port' => $port
	);

	$playerList[ $name ] = $player;

	return $player;
}

require_once( 'game.inc' );

error_reporting(E_ALL);

/* Allow the script to hang around waiting for connections. */
set_time_limit(0);

/* Turn on implicit output flushing so we see what we're getting
 * as it comes in. */
ob_implicit_flush();

$address = '192.168.55.33';
$port = 4609;

if (($sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
	echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
}

if (socket_bind($sock, $address, $port) === false) {
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
	/* Send instructions. */
	//$msg = "\nWelcome to the PHP Test Server. \n" .
		//"To quit, type 'quit'. To shut down the server type 'shutdown'.\n";
	//socket_write($msgsock, $msg, strlen($msg));
//
	//do {
		//if (false === ($buf = socket_read($msgsock, 2048, PHP_NORMAL_READ))) {
			//echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
			//break 2;
		//}
		//if (!$buf = trim($buf)) {
			//continue;
		//}
		//if ($buf == 'quit') {
			//break;
		//}
		//if ($buf == 'shutdown') {
			//socket_close($msgsock);
			//break 2;
		//}
		//$talkback = "PHP: You said '$buf'.\n";
		//socket_write($msgsock, $talkback, strlen($talkback));
		//echo "$buf\n";
	//} while (true);
	//socket_close($msgsock);
	
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

	case 'register':
		$player = register_player( 	(string)$request->message->name,
									(string)$request->message->ip,
									(string)$request->message->port );

		echo "Registered $player->name\n";
		break;

	case 'start_game':

		break;

	case 'play_tiles':
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
