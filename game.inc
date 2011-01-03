<?php

require_once( 'board.inc' );
require_once( 'bag.inc' );
require_once( 'player.inc' );

class Game {
	public $board;
	public $bag;
	public $players;

	function __construct() {
		$board = new Board();
		$bag = new Bag();
		$players = array();
	}
}

?>
