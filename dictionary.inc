<?php
class Dictionary {
	public $words;

	function __construct() {
		$file = fopen( 'word_list.txt', 'r' );

		while( $word = fread( $file ) ) {
			$words[ $word ] = $word;
		}
	}

	// Return true if word is in dictionary, otherwise false.
	function check_word( $word ) {
		if( isset( $words[ $word ] )  ) {
			return true;
		} 

		return false;
	}
}
?>
