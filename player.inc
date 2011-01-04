<?php

class Player {
	public $name;
	public $ip;
	public $port;
	public $hand;

	function __construct( $name, $ip, $port ) {
		$this->name = $name;
		$this->ip = $ip;
		$this->port = $port;

		$this->hand = array();
	}

	function add_tile_to_hand( $tile ) {
		$this->hand[] = $tile;
	}

	// Returns true if the player has the given letter in his hand, otherwise returns false.
	function has_letter( $letter ) {
		foreach( $this->hand as $tile ) {
			if( $tile->letter == $letter ) {
				return true;
			}
		}

		return false;
	}

	function hand_to_xml() {
		$xml = "<hand>";

		foreach( $this->hand as $tile ) {
			$xml .= 
				"<tile>".
					"<letter>$tile->letter</letter>".
					"<value>$tile->value</value>".
				"</tile>".
				"";
		}

		$xml .= "</hand>";

		return $xml;
	}
}

?>
