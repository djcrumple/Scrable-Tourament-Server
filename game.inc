<?php

require_once( 'board.inc' );
require_once( 'bag.inc' );
require_once( 'player.inc' );

// This will be used to figure out the ID of new games.
// As we create games, we will increment this number. 
$nextId = 0;

class Game {
	// The ID of the game.
	public $id;
	// The game board.
	public $board;
	// The bag of tiles.
	public $bag;
	// List of players.
	public $players;
	// The index of the active player. If this game has not started this will
	// be === false.
	public $currentPlayer;


	function __construct() {
		global $nextId;

		$this->id = $nextId;
		$nextId++;

		$this->board = new Board();
		$this->bag = new Bag();
		$this->players = array();

		// We will pick a random player to be the current player after
		// all players have been added.
		$this->currentPlayer = false;
	}

	function add_player( $name, $ip, $port ) {
		// If a current player already exists, that means the game is started and we
		// cannot add anymore players.
		if( $this->currentPlayer !== false ) {
			echo "ERROR: Cannot add player. Game has already started.\n";
			return false;
		}
		// Create the player.
		$player = new Player( $name, $ip, $port );

		// Give the player a hand by getting 7 tiles from the bag.
		for( $i = 0; $i < 7; $i++ ) {
			$tile = $this->bag->draw_tile();
			$player->add_tile_to_hand( $tile );
		}

		$this->players[] = $player;
	}

	function next_turn() {
		if( $currentPlayer === false ) {
			// This is the first turn of the game. Shuffle the players.
			shuffle( $players );
			$currentPlayer = 0;
		}


	}

}

?>
