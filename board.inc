<?php


require_once( 'dictionary.inc' );

$dictionary = false;

class Board {
	const SPACE_TYPE_EMPTY 			= '0';
	const SPACE_TYPE_DOUBLE_LETTER 	= '1';
	const SPACE_TYPE_TRIPLE_LETTER 	= '2';
	const SPACE_TYPE_DOUBLE_WORD 	= '3';
	const SPACE_TYPE_TRIPLE_WORK	= '4';

	public $size;
	public $spaces;

	function __construct() {
		$this->size = 15;
		$this->spaces = array();

		for( $i = 0; $i < $this->size; $i++ ) {
			$this->spaces[ $i ] = array();

			for( $j = 0; $j < $this->size; $j++ ) {
				$this->spaces[$i][$j] = self::SPACE_TYPE_EMPTY;
			}
		}	
	}

	function to_xml() {
		$xml = "<board>";

		for( $i = 0; $i < $this->size; $i++ ) {
			for( $j = 0; $j < $this->size; $j++ ) {
				$xml .= $this->spaces[$i][$j];
			}
		}	

		$xml .= "</board>";
		
		return $xml;
	}

	function play_word( $word, $x1, $y1, $x2, $y2 ) {
		global $dictionary;

		if( $dictionary === false ) {
			$dictionary = new Dictionary();
		}


	}


}

?>
