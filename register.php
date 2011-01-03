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
		"<register>".
		"<name>Player$i</name>".
		"<ip>$myIp</ip>".
		"<port>$myPort</port>".
		"</register>\n".
		"";

	if( false === fwrite( $file, $register ) ) {
		echo "Write failed\n";
	}
}


?>
