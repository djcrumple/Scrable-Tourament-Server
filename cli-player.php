#! /usr/bin/php
<?php

$serverIp = '127.0.0.1';
$serverPort = 4609;

$myIp = '127.0.0.1';
$myPort = 4610;

function usage() {
	echo "Usage:\n";
	echo "   {$_SERVER['argv'][0]} --server <ip>:<port> --client <ip>:<port>\n";
	exit( 1 );
}

for( $i = 1; $i < $_SERVER['argc']; $i++ ) {
	$parameter = $_SERVER['argv'][ $i ];

	switch( $parameter ) {
		case '--client':
			if( $i+1 >= $_SERVER['argc'] ) {
				usage();
			}
			$parameter = $_SERVER['argv'][ ++$i ];
			$array = explode( ':', $parameter );
			$myIp = trim( $array[ 0 ] );
			$myPort = trim( $array[ 1 ] );
			break;
		case '--server':
			if( $i+1 >= $_SERVER['argc'] ) {
				usage();
			}
			$parameter = $_SERVER['argv'][ ++$i ];
			$array = explode( ':', $parameter );
			$serverIp = trim( $array[ 0 ] );
			$serverPort = trim( $array[ 1 ] );
			break;
	}
}

error_reporting(E_ALL);

/* Allow the script to hang around waiting for connections. */
set_time_limit(0);

/* Turn on implicit output flushing so we see what we're getting
 * as it comes in. */
ob_implicit_flush();

$STDIN = fopen( "php://stdin", "r" );

function printBoard( $board ) {
	echo "    ";
	for( $x = 0; $x < 15; $x++ ) {
		echo " ".sprintf( "%2d", $x );
	}
	echo "\n";
	for( $y = 0; $y < 15; $y++ ) {
		echo sprintf("%2d", $y ).": ";
		for( $x = 0; $x < 15; $x++ ) {
			$letter = $board[ $x*15 + $y ];
			echo "[".( ( is_numeric($letter) && $letter == 0 ) ? ' ' : strtoupper($letter) )."]"; 
		}
		echo "\n";
	}
}
function printHand( $hand ) {
	echo "Hand: ";
	for( $i = 0; $i < sizeof( $hand ); $i++ ) {
		$letter = $hand[$i]->letter;
		echo " ".strtoupper( $letter )." ";
	}
	echo "\n";
	echo "      ";
	for( $i = 0; $i < sizeof( $hand ); $i++ ) {
		$value = $hand[$i]->value;
		echo sprintf( "%2s", $value )." ";
	}
	echo "\n";
}



// Register myself.
$file = fsockopen( $serverIp, $serverPort );
if( !$file ) {
	echo "ERROR: Could not open socket to server.\n";
	exit( 1 );
}

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
	exit( 1 );
}

if (socket_bind($sock, $myIp, $myPort) === false) {
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

	case 'ping':
		echo "I've been pinged\n"; 

		// After being pinged, I have to return 'hello' to let the server know
		// that I am here.
		$resp = "hello\n";

		socket_write( $msgsock, $resp, strlen( $resp ) );
		break;

	case 'game_state':
		
		$board = (string)$request->message->board;
		$hand = array();
		foreach( $request->message->hand->tile as $tile ) {
			$hand[] = (object)array( 'letter' => (string)$tile->letter, 'value' => (string)$tile->value );
		}

		do {
			printBoard( $board );
			echo "\n";
			printHand( $hand );
			echo "\n";

			echo "Letters? ";
			$letterString = strtolower( trim( fgets( $STDIN ) ) );
			$letters = array();
			$realLetters = 0;
			if( strlen($letterString) > 0 ) {
				for( $i = 0; $i < strlen( $letterString ); $i++ ) {
					$letter = $letterString[ $i ];
					if( $letter == '*' ) {
						$letter = $letterString[ ++$i ];
						$letters[] = (object)array( 'letter' => $letter, 'blank' => 1 );
						$realLetters++;
					} else if( $letter == '-' ) {
						$letters[] = false;
					} else {
						$letters[] = (object)array( 'letter' => $letter, 'blank' => 0 );
						$realLetters++;
					}
				}

				do {
					echo "Location 'x,y'? ";
					$location = explode( ',', fgets( $STDIN ) );
					$xLocation = trim( $location[ 0 ] );
					$yLocation = trim( $location[ 1 ] );
				} while( $xLocation < 0 || $xLocation > 14 || $yLocation < 0 || $yLocation > 14 );
				do {
					echo "Direction [d/r]? ";
					$direction = trim( fgets( $STDIN ) );
				} while( !in_array($direction,array('d','r')) );
			}

			echo "Letters(".$realLetters."): ".print_r( $letters, 1 )."\n";
			if( $letters ) {
				echo "At: ( {$xLocation}, {$yLocation} )\n";
				echo "Direction: {$direction}\n";
			}

			$newHand = $hand;
			$validMoves = 0;
			foreach( $letters as $letter ) {
				if( !$letter ) {
					continue;
				}
				foreach( $newHand as $h => $tile ) {
					if( ( $letter->blank == 1 && $tile->letter == '*' ) || ( $letter->blank == 0 && $letter->letter == $tile->letter ) ) {
						array_splice( $newHand, $h, 1 );
						$validMoves++;
						break;
					}
				}
			}

			$okay = 'n';
			if( $validMoves == $realLetters ) {
				$newBoard = $board;
				for( $i = 0; $i < sizeof($letters); $i++ ) {
					if( !$letters[$i] ) {
						continue;
					}

					if( $direction == 'r' ) {
					// Left to right
						$x = $xLocation + $i;
						$y = $yLocation;
					} else {
					// Top to bottom
						$x = $xLocation;
						$y = $yLocation + $i;
					}
					$newBoard[ $x*15 + $y ] = $letters[ $i ]->letter;
				}

				printBoard( $newBoard );
				echo "\n";
				printHand( $newHand );

				do {
					echo "Okay [y/n]? ";
					$okay = trim( fgets( $STDIN ) );
				} while( !in_array($okay,array('y','n')) );
			}
		} while( $okay != 'y' );


		$file = fsockopen( $serverIp, $serverPort );

		$req = 
			"<request>".
				"<type>play</type>".
				"<message>".
					"<hash>".(string)$request->message->hash."</hash>".
					"<game_id>".(string)$request->message->game_id."</game_id>".
			"";
		for( $i = 0; $i < sizeof($letters); $i++ ) {
			if( !$letters[$i] ) {
				continue;
			}
			if( $direction == 'r' ) {
			// Left to right
				$x = $xLocation + $i;
				$y = $yLocation;
			} else {
			// Top to bottom
				$x = $xLocation;
				$y = $yLocation + $i;
			}
			$req .=
				"<move>".
					"<letter>".$letters[$i]->letter."</letter>".
					"<x>{$x}</x>".
					"<y>{$y}</y>".
					"<blank>".$letters[$i]->blank."</blank>".
				"</move>".
				"";
		}
		$req .=
				"</message>".
			"</request>".
			"\n".
			"";

		echo "Request: {$req}\n";

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
