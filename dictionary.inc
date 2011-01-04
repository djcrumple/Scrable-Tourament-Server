<?php
class Dictionary {
	public $words;

	function __construct() {
		$file = fopen( 'word_list.txt', 'r' );

		while( $word = fgets( $file ) ) {
			$word = trim( $word );
			//echo $word."\n";
			$this->words[ $word ] = $word;
		}
	}

	// Return true if word is in dictionary, otherwise false.
	function check_word( $word ) {
		if( isset( $this->words[ $word ] )  ) {
			return true;
		} 

		return false;
	}
}
?>
