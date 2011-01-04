<?php

$values = array(
	'*' => 0,
	'a' => 1,
	'b' => 3,
	'c' => 3,
	'd' => 2,
	'e' => 1,
	'f' => 2,
	'g' => 3,
	'h' => 4,
	'i' => 1,
	'j' => 8,
	'k' => 5,
	'l' => 1,
	'm' => 3,
	'n' => 1,
	'o' => 1,
	'p' => 3,
	'q' => 10,
	'r' => 1,
	's' => 1,
	't' => 1,
	'u' => 1,
	'v' => 4,
	'w' => 4,
	'x' => 1,
	'y' => 4,
	'z' => 10 
);

class Tile {
	public $letter;
	public $value;

	public $isWild;


	function __construct( $letter ) {
		global $values;

		$this->letter = $letter;
		$this->value = $values[ $letter ];
		$this->isWild = false;

		if( $letter == '*' ) {
			$this->isWild = true;
		}

	}
}

?>
