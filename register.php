#! /usr/bin/php 
<?

$serverIp = '192.168.55.33';
$serverPort = '4609';

$myIp = '192.168.55.33';
$myPort = '4610';

// The number of players to register.
$numPlayers = 4;

for( $i = 1; $i <= 4; $i++ ) {

	$file = fsockopen( $serverIp, $serverPort );

	$register = 
		"<request>".
			"<type>register</type>".
			"<message>".
				"<name>Player$i</name>".
				"<ip>$myIp</ip>".
				"<port>$myPort</port>".
			"</message>".
		"</request>".
		"\n".
		"";

	if( false === fwrite( $file, $register ) ) {
		echo "Write failed\n";
	}
}


?>
