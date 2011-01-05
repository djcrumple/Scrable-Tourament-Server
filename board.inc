<?php


require_once( 'dictionary.inc' );
require_once( 'space.inc' );


$dictionary = false;

class Board {

	public $size;
	public $spaces;

	function __construct() {
		$this->size = 15;
		$this->spaces = array();

		for( $i = 0; $i < $this->size; $i++ ) {
			$this->spaces[ $i ] = array();

			for( $j = 0; $j < $this->size; $j++ ) {
				$this->spaces[$i][$j] = new Space();
			}
		}	

		// Set all of the special spaces.

		// Double letter.
		$this->spaces[0][3]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[0][11]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[2][6]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[2][8]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[3][0]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[3][7]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[3][14]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[6][2]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[6][6]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[6][8]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[6][12]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[7][3]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[7][11]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[8][2]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[8][6]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[8][8]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[8][12]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[11][0]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[11][7]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[11][14]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[12][6]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[12][8]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[14][3]->type = Space::TYPE_DOUBLE_LETTER;
		$this->spaces[14][11]->type = Space::TYPE_DOUBLE_LETTER;

		// Double Word
		$this->spaces[1][1]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[1][13]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[2][2]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[2][12]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[3][3]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[3][11]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[4][4]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[4][10]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[7][7]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[10][4]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[10][10]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[11][3]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[11][11]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[12][2]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[12][12]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[13][1]->type = Space::TYPE_DOUBLE_WORD;
		$this->spaces[13][13]->type = Space::TYPE_DOUBLE_WORD;

		// Triple Letter
		$this->spaces[1][5]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[1][9]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[5][1]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[5][5]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[5][9]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[5][13]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[9][1]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[9][5]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[9][9]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[9][13]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[13][5]->type = Space::TYPE_TRIPLE_LETTER;
		$this->spaces[13][9]->type = Space::TYPE_TRIPLE_LETTER;

		// Triple Word
		$this->spaces[0][0]->type = Space::TYPE_TRIPLE_WORD;
		$this->spaces[0][7]->type = Space::TYPE_TRIPLE_WORD;
		$this->spaces[0][14]->type = Space::TYPE_TRIPLE_WORD;
		$this->spaces[7][0]->type = Space::TYPE_TRIPLE_WORD;
		$this->spaces[7][14]->type = Space::TYPE_TRIPLE_WORD;
		$this->spaces[14][0]->type = Space::TYPE_TRIPLE_WORD;
		$this->spaces[14][7]->type = Space::TYPE_TRIPLE_WORD;
		$this->spaces[14][14]->type = Space::TYPE_TRIPLE_WORD;
	}

	// Convert the board to XML. The board will be represented as a string surrounded by <board>
	// tags. The string will have a single character per space. The character will be the letter
	// of the tile if a tile is in the space. Otherwise, the character will represent the type
	// of space. The type of space is expressed as a number between 0 and 4. Refer to space.inc
	// for those values.
	//
	// The first character will represent [0][0] on the board. The next character is [0][1]. 
	// The sixteenth character represents the top of the next column on the board( [1][0] ).
	// The final character is the bottom right of the board ( [14][14] ).
	function to_xml() {
		$xml = "<board>";

		for( $i = 0; $i < $this->size; $i++ ) {
			for( $j = 0; $j < $this->size; $j++ ) {
				if( $this->spaces[$i][$j]->is_empty() ) {
					// If there's no letters in this place, but the type of space instead.
					$xml .= $this->spaces[$i][$j]->type;
				} else {
					// Found a letter. Add this letter to the string. 
					$xml .= $this->spaces[$i][$j]->tile->letter;
				}
			}
		}	

		$xml .= "</board>";
		
		return $xml;
	}

	// Tries to put the given tiles on the board. If the play is illegal, then false is returned.
	// Note that it is possible to get a score of 0 on the first turn by playing two blank tiles.
	function play( $moves ) {
		global $dictionary;

		if( $dictionary === false ) {
			$dictionary = new Dictionary();
		}

		// Figure out if the word is going right or down. Do this by figuring out the min/max x
		// and y from the given tiles.
		$wordDir = false;
		$minX = $this->size;
		$minY = $this->size;
		$maxX = 0;
		$maxY = 0;
		foreach( $moves as $move ) {
			if( $move->x < $minX ) {
				$minX = $move->x;
			}
			if( $move->x > $maxX ) {
				$maxX = $move->x;
			}
			if( $move->y < $minY ) {
				$minY = $move->y;
			}
			if( $move->y > $maxY ) {
				$maxY = $move->y;
			}
		}

		// A single tile will be considered down and right, but it doesn't matter which 
		// one we pick.
		if( $minX == $maxX ) {
			$wordDir = "down";
		}
		if( $minY == $maxY ) {
			$wordDir = "right";
		}

		// If the word does not have a directon, then the play is illegal.
		if( $wordDir === false ) {
			echo "ERROR: Tiles are not in a line.\n";
			return false;
		}

		// Make a copy of the board. We will try to play the tiles on the copy to check legality of play.
		$copy = $this->spaces;

		foreach( $moves as $move ) {
			if( !( $copy[ $move->x ][ $move->y ]->is_empty() ) ) {
				// This space is not empty.
				echo "ERROR: Title already exists in this space.\n";
				return false;
			}

			// Space is empty, place move.
			$copy[ $move->x ][ $move->y ]->tile = $move->tile;
		}

		// Make sure the center space as a tile.
		$centerX = floor( $this->size / 2 );
		$centerY = floor( $this->size / 2 );
		if( $copy[ $centerX ][ $centerY ]->is_empty() ) {
			echo "ERROR: Center space is empty.\n";
			return;
		}

		// All the letters have been placed. Now we can score the word. This will also make sure all words
		// built from these tiles are legal.
		$score = 0;
		if( $wordDir == 'right' ) {
			$word = "";
			$x = $minX;
			$y = $minY;

			// This will be the product of all word bonuses. 2 double word bonuses would 
			// make this 2 x 2 = 4
			$wordBonus = 1;
			// The sum of all the letter bonuses. 
			$letterBonus = 0;
			// The score of the current word. Multiple words may be formed in a single play.
			$wordScore = 0;


			// Find the first letter in the word.
			while( $x > 0 && !( $copy[ $x - 1 ][ $y ]->is_empty() ) ) {
				$x--;
			}
			
			// From the first letter, build the word.
			while( $x < $this->size - 1 && !( $copy[ $x ][ $y ]->is_empty() ) ) {
				$space = $copy[ $x ][ $y ];
				$tile = $space->tile;

				// Figure out the score.
				switch( $space->type ) {
				case Space::TYPE_DOUBLE_LETTER:
					$wordScore += $tile->value; 
					break;
				case Space::TYPE_TRIPLE_LETTER:
					$wordScore += $tile->value * 2; 
					break;
				case Space::TYPE_DOUBLE_WORD:
					$wordBonus *= 2;
					break;
				case Space::TYPE_TRIPLE_WORD:
					$wordBonus *= 3;
					break;
			
				}
				$wordScore += $tile->value;

				$word .= $tile->letter;

				$x++;
			}

			if( strlen( $word ) > 1 ) {

				if( $x < $maxX ) {
					// There is a gap in the tiles.
					echo "ERROR: Gap in the tiles.\n";
					return false;
				}

				// Make sure the word is in the dictionary. 
				if( !( $dictionary->check_word( $word ) ) ) {
					echo "ERROR: '$word' is not a word.\n";
					return false;
				}

				// Calculate the score of the word and add it to the total score for this play.
				$wordScore *= $wordBonus;
				$score += $wordScore;

				echo "'$word' scored $wordScore\n";
			}

			// Now go through all of the words that were built perdindicular. 
			foreach( $moves as $move ) {
				// Reset the word.
				$word = "";
				$wordBonus = 1;
				$wordScore = 0;

				$x = $move->x;
				$y = $move->y;

				// Find the first letter in the word.
				while( $y != 0 && !( $copy[ $x ][ $y - 1 ]->is_empty() ) ) {
					$y++;
				}

				// From the first letter, build the word.
				while( $y < $this->size - 1 && !( $copy[ $x ][ $y ]->is_empty() ) ) {
					$space = $copy[ $x ][ $y ];
					$tile = $space->tile;

					// Figure out the score.
					switch( $space->type ) {
					case Space::TYPE_DOUBLE_LETTER:
						$wordScore += $tile->value; 
						break;
					case Space::TYPE_TRIPLE_LETTER:
						$wordScore += $tile->value * 2; 
						break;
					case Space::TYPE_DOUBLE_WORD:
						$wordBonus *= 2;
						break;
					case Space::TYPE_TRIPLE_WORD:
						$wordBonus *= 3;
						break;

					}
					$wordScore += $tile->value;

					$word .= $tile->letter;

					$y++;
				}

				if( strlen( $word ) > 1 ) {
					// Make sure the word is in the dictionary. 
					if( !( $dictionary->check_word( $word ) ) ) {
						echo "ERROR: '$word' is not a word.\n";
						return false;
					}

					// Calculate the score of the word and add it to the total score for this play.
					$wordScore *= $wordBonus;
					$score += $wordScore;

					echo "'$word' scored $wordScore\n";
				}
			}
		} else {
			$word = "";
			$x = $minX;
			$y = $minY;

			// This will be the product of all word bonuses. 2 double word bonuses would 
			// make this 2 x 2 = 4
			$wordBonus = 1;
			// The sum of all the letter bonuses. 
			$letterBonus = 0;
			// The score of the current word. Multiple words may be formed in a single play.
			$wordScore = 0;


			// Find the first letter in the word.
			while( $y > 0 && !( $copy[ $x ][ $y - 1 ]->is_empty() ) ) {
				$y--;
			}
			
			// From the first letter, build the word.
			while( $y < $this->size - 1 && !( $copy[ $x ][ $y ]->is_empty() ) ) {
				$space = $copy[ $x ][ $y ];
				$tile = $space->tile;

				// Figure out the score.
				switch( $space->type ) {
				case Space::TYPE_DOUBLE_LETTER:
					$wordScore += $tile->value; 
					break;
				case Space::TYPE_TRIPLE_LETTER:
					$wordScore += $tile->value * 2; 
					break;
				case Space::TYPE_DOUBLE_WORD:
					$wordBonus *= 2;
					break;
				case Space::TYPE_TRIPLE_WORD:
					$wordBonus *= 3;
					break;
			
				}
				$wordScore += $tile->value;

				$word .= $tile->letter;

				$y++;
			}

			if( strlen( $word ) > 1 ) {

				if( $y < $maxY ) {
					// There is a gap in the tiles.
					echo "ERROR: Gap in the tiles.\n";
					return false;
				}

				// Make sure the word is in the dictionary. 
				if( !( $dictionary->check_word( $word ) ) ) {
					echo "ERROR: '$word' is not a word.\n";
					return false;
				}

				// Calculate the score of the word and add it to the total score for this play.
				$wordScore *= $wordBonus;
				$score += $wordScore;

				echo "'$word' scored $wordScore\n";
			}

			// Now go through all of the words that were built perdindicular. 
			foreach( $moves as $move ) {
				// Reset the word.
				$word = "";
				$wordBonus = 1;
				$wordScore = 0;

				$x = $move->x;
				$y = $move->y;

				// Find the first letter in the word.
				while( $x != 0 && !( $copy[ $x - 1 ][ $y ]->is_empty() ) ) {
					$x++;
				}

				// From the first letter, build the word.
				while( $x < $this->size - 1 && !( $copy[ $x ][ $y ]->is_empty() ) ) {
					$space = $copy[ $x ][ $y ];
					$tile = $space->tile;

					// Figure out the score.
					switch( $space->type ) {
					case Space::TYPE_DOUBLE_LETTER:
						$wordScore += $tile->value; 
						break;
					case Space::TYPE_TRIPLE_LETTER:
						$wordScore += $tile->value * 2; 
						break;
					case Space::TYPE_DOUBLE_WORD:
						$wordBonus *= 2;
						break;
					case Space::TYPE_TRIPLE_WORD:
						$wordBonus *= 3;
						break;

					}
					$wordScore += $tile->value;

					$word .= $tile->letter;

					$x++;
				}

				if( strlen( $word ) > 1 ) {
					// Make sure the word is in the dictionary. 
					if( !( $dictionary->check_word( $word ) ) ) {
						echo "ERROR: '$word' is not a word.\n";
						return false;
					}

					// Calculate the score of the word and add it to the total score for this play.
					$wordScore *= $wordBonus;
					$score += $wordScore;

					echo "'$word' scored $wordScore\n";
				}
			}
		}

		if( count( $moves ) == 7 ) {
			$score += 50;
		}

		// The move was legal, assign the copy to the board.
		$this->spaces = $copy;

		return $score;
	}


}

?>
