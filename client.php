#! /usr/bin/php 
<?
$file = fsockopen( '192.168.55.33', 4609 );
$input = fopen( 'php://stdin', 'r' );

$string = fgets( $file );

echo $string."\n";



while( 1 ) {
	$string = fgets( $input );

	fwrite( $file, $string );

	$string = fgets( $file );

	echo $string."\n";
}

?>
