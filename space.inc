<?php

require_once( 'tile.inc' );

// This represents a space on the scrabble board. It has a type (like Double Letter or Triple Word).
// It can also contain a tile.
class Space {

	const SPACE_TYPE_NO_BONUS 		= '0';
	const SPACE_TYPE_DOUBLE_LETTER 	= '1';
	const SPACE_TYPE_TRIPLE_LETTER 	= '2';
	const SPACE_TYPE_DOUBLE_WORD 	= '3';
	const SPACE_TYPE_TRIPLE_WORK	= '4';

	// The type of space. The type
	public $type;
	// Empty spaces will have tile === false.
	public $tile;

	function __construct() {
		$this->type = self::SPACE_TYPE_NO_BONUS;
		$this->tile = false;
	}


	// Return true if the space does not have a tile.
	function is_empty() {
		return ( $this->tile === false );
	}

}

?>


