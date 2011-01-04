#! /usr/bin/php 
<?

$serverIp = '192.168.55.33';
$serverPort = '4609';

$file = fsockopen( $serverIp, $serverPort );

/*
$req = 
	"<request>".
		"<type>start_game</type>".
		"<message>".
			"<player>Player1</player>".
			"<player>Player2</player>".
			"<player>Player3</player>".
			"<player>Player4</player>".
		"</message>".
	"</request>".
	"\n".
	"";
//*/

//*
$req = 
	"<request>".
		"<type>start_game</type>".
		"<message>".
			"<player>TEST_CLIENT</player>".
		"</message>".
	"</request>".
	"\n".
	"";
//*/

if( false === fwrite( $file, $req ) ) {
	echo "Write failed\n";
}


?>
