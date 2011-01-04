#! /usr/bin/php

<?php

// List of all registered players indexed by their name. For each player
// we will know their IP and Port number.
$registeredPlayers = array();

function register_player( $name, $ip, $port ) {
	global $registeredPlayers;

	$player = (object)array(
		'name' => $name,
		'ip' => $ip,
		'port' => $port
	);

	$registeredPlayers[ $name ] = $player;

	return $player;
}

function ping_player( $player ) {
	$file = fsockopen( $player->ip, $player->port );

	$ping = "<request><type>ping</type></request>\n";

	if( false === fwrite( $file, $ping ) ) {
		echo "Write failed\n";
		return false;
	}

	stream_set_timeout( $file, 2 );

	$resp = trim( fread( $file, 16 ) );

	if( $resp != 'hello' ) {
		return false;
	}

	return true;
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
	exit( 1 );
}

if (socket_bind($sock, $address, $port) === false) {
	echo "socket_bind() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
	exit( 1 );
}

if (socket_listen($sock, 5) === false) {
	echo "socket_listen() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
	exit( 1 );
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

	case 'register':
		$player = register_player( 	(string)$request->message->name,
									(string)$request->message->ip,
									(string)$request->message->port );

		echo "Registered $player->name\n";
		break;

	case 'start_game':
		print_r( $registeredPlayers );
		// The list of players that will be added to the new game.
		// The players must have been registered.
		$gamePlayers = array();
		foreach( $request->message->player as $playerName ) {
			// Make sure playerName is not a SimpleXMLElement.
			$playerName = (string)$playerName;

			if( !$registeredPlayers[ $playerName ] ) {
				echo "ERROR: $playerName is not registered\n";
				continue;
			}
			$gamePlayers[] = $registeredPlayers[ $playerName ];
		}

		if( count( $gamePlayers ) < 1 ) {
			echo "ERROR: Not enough registered players\n";
		}

		// Make sure all players are available
		$missingPlayer = false;
		foreach( $gamePlayers as $player ) { 
			$available = ping_player( $player );

			if( $available ) {
				echo "$player->name is available\n";
			} else {
				echo "ERROR: $player->name is not available\n";
				$missingPlayer = true;
				break;
			}
		}

		if( $missingPlayer ) {
			echo "ERROR: Cannot start game, player missing.\n";
		}

		// Create the game
		$game = new Game( $gamePlayers );

		// Add all of the registered players to the game.
		foreach( $gamePlayers as $player ) { 
			$game->add_player( $player->name, $player->ip, $player->port );
		}

		print_r( $game );

		$game->next_turn();
		break;

	case 'play':
		// The array of moves that the player is attempting to make in this play.
		$moves = array();

		// Get the tile info out of the XML.
		foreach( $request->message->move as $xmlMove) {
			$move = (object)array(
				'tile' => new Tile( (string)$xmlMove->letter ),
				'x' => (int)$xmlMove->x, 
				'y' => (int)$xmlMove->y
			);

			// TODO: Set $move->tile->isWild based on XML.
			if( isset( $xmlMove->blank ) && $xmlMove->blank == '1' ) {
				$move->tile->isWild = 1;
				$move->tile->value = 0;
			}

			$moves[] = $move;
		}

		// We don't care if this fails, if the player made a bad move then they lose 
		// their turn.
		$game->play( $moves );

		sleep( 30 );

		// Send the next player the game state.:q
		$game->next_turn();

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
