<?php

require_once( 'tile.inc' );

$distribution = array(
	'*' => 2,
	'a' => 9,
	'b' => 2,
	'c' => 2,
	'd' => 4,
	'e' => 12,
	'f' => 2,
	'g' => 3,
	'h' => 2,
	'i' => 9,
	'j' => 1,
	'k' => 1,
	'l' => 4,
	'm' => 2,
	'n' => 6,
	'o' => 8,
	'p' => 2,
	'q' => 1,
	'r' => 6,
	's' => 4,
	't' => 6,
	'u' => 4,
	'v' => 2,
	'w' => 2,
	'x' => 1,
	'y' => 2,
	'z' => 1
);


class Bag {
	public $tiles;

	function __construct() {
		global $distribution;

		// Fill the bag.
		foreach( $distribution as $letter => $num ) {
			for( $i = 0; $i < $num; $i++ ) {
				$tile = new Tile( $letter );
				$this->tiles[] = $tile;
			}
		}

		// Randomize the tiles.
		shuffle( $this->tiles );
		
	}

	// Get a random tile from the bag. Return false if the bag is empty.
	function draw_tile() {
		$tile = array_pop( $this->tiles );

		if( !$tile ) {
			return false;
		}

		return $tile;
	}

	// Put a tile back in the bag.
	function return_tile( $tile ) {
		// TODO:
		//   Make sure there are not too many letter.
	
		$this->tiles[] = $tile;

		// Randomize the tiles.
		shuffle( $this->tiles );
	}

	// Get the number of tiles left in the bag.
	function get_num_tiles() {
		return count( $this->tiles );
	}
}
?>
