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
}

?>
