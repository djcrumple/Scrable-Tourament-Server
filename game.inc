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


	/**
	 * Make a play in the game. A play consists of a series of moves. Each move places
	 * a tile on the board. $moves has the following format.
	 *     $moves = array(
	 *         (object)array(
	 *             'tile' => The tile to be played.
	 *             'x' => The x position on the board to place the tile.
	 *             'y' => The y position on the board to place the tile.
	 *
	 *     );
	 *
	 * Scrabble game is cruel and unforgiving. If the move is bad, the players turn is skipped.
	 **/
	function play( $moves ) {
		// Grab the current player.
		$player = $this->players[ $this->currentPlayer ];

		// See if the player has to tiles he is trying to play.
		//foreach( $moves as $move ) {
			//if( !$player->has_letter( $move->tile->letter ) ) {
				//return false;
			//}
		//}

		// Pass the moves to the board.
		$score = $this->board->play( $moves );

		if( $score === false ) {
			echo "ERROR: Illegal play.\n";
		} else {
			echo "Play was worth $score points\n";
		}

	}

	function next_turn() {
		if( $this->currentPlayer === false ) {
			// This is the first turn of the game. Shuffle the players.
			shuffle( $this->players );
			$this->currentPlayer = 0;
		} else {
			$this->currentPlayer++;

			if( $this->currentPlayer = count( $this->players ) ) {
				$this->currentPlayer = 0;
			}
		}

		$boardXml = $this->board->to_xml();

		$handXml = $this->players[ $this->currentPlayer ]->hand_to_xml();

		//echo $boardXml."\n";
		//echo $handXml."\n";

		$req = 
			"<request>".
				"<type>game_state</type>".
				"<message>".
					"<game_id>$this->id</game_id>".
					"$boardXml".
					"$handXml".
				"</message>".
			"</request>".
			"\n";

		$ip = $this->players[ $this->currentPlayer ]->ip;
		$port = $this->players[ $this->currentPlayer ]->port;

		$file = fsockopen( $ip, $port );

		if( false === fwrite( $file, $req ) ) {
			echo "ERROR: Could not register.\n";
		}
		
	}

}

?>
