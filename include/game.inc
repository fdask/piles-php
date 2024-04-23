<?php
class Game {
	private $assembler;
    private $conveyor;
    private $status;

    private $ticks;
    private $display;
	private $term;

	public function __construct() {
        $this->term = `stty -g`;
		system("stty -icanon -echo");

        echo "\n";

		$this->ticks = 0;

        $this->conveyor = new Conveyor();
        $this->assembler = new Assembler();
        $this->status = new Status();

		stream_set_blocking(STDIN, false); // Do not wait

        $this->draw();

		$this->gameLoop();
	}

	public function gameLoop() {
        // start of INFINITE LOOP
		while (1) {
			$this->ticks++;
		
			if ($this->ticks == Config::$tickLen) {
                $this->tick();

                $this->ticks = 0;
			
                $this->draw();
			}
		
            // start of HANDLE KEYPRESS
			$c = Config::waitForInput();
		
			switch ($c) {
				case Config::keypressUpArrow():
					$this->conveyor->moveUp();

                    $this->draw();

					break;
                case Config::keypressShiftUpArrow():
                    $this->conveyor->addCounter(Pile::$behaviorUp);

                    $this->draw();

                    break;
                case Config::keypressDownArrow():
                    $this->conveyor->moveDown();
                
                    $this->draw();

                    break;
                case Config::keypressShiftDownArrow():
                    $this->conveyor->addCounter(Pile::$behaviorDown);

                    $this->draw();

                    break;
                case Config::keypressLeftArrow():
                    $this->conveyor->moveLeft();

                    $this->draw();

                    break;
                case Config::keypressShiftLeftArrow():
                    $this->conveyor->addCounter(Pile::$behaviorLeft);

                    $this->draw();

                    break;
                case Config::keypressRightArrow():
                    $this->conveyor->moveRight();

                    $this->draw();

                    break;
                case Config::keypressShiftRightArrow():
                    $this->conveyor->addCounter(Pile::$behaviorRight);

                    $this->draw();

                    break;
                case 'q':
                    $this->gameOver();
                    exit;
				default:
					// do nothing
			}
            // stop of HANDLE KEYPRESS
		}		
        // stop of INFINITE LOOP
	}

    public function tick() {
        $this->conveyor->tick();
        $this->assembler->tick();
        $this->status->tick();

        // check if the assembler is completed
        if ($this->assembler->isDone()) {
            $pile = $this->assembler->getPile();
            
            $popped = $this->conveyor->addPile($pile);

            // check the pile that popped off
            if (!is_null($popped)) {
                if ($popped->isCountered()) {
                    $score = 0;

                    // pile is properly countered
                    $behaviors = $popped->getBehaviors();

                    if (in_array(Pile::$behaviorRight, $behaviors) || in_array(Pile::$behaviorLeft, $behaviors)) {
                        $score += Config::$leftRightScore;
                    }
                    
                    if (in_array(Pile::$behaviorUp, $behaviors) || in_array(Pile::$behaviorDown, $behaviors)) {
                        $score += Config::$upDownScore;
                    }
                    
                    $msg = Colors::fg_color('green', "ok!");

                    if ($score > 0) {
                        $msg .= " +$score";
                        $this->status->addScore($score);
                    }

                    $this->status->addMsg($msg);
                    $this->status->addHit();
                    $this->draw();
                } else {
                    $this->status->addMiss();
                    $this->status->addMsg(Colors::fg_color('red', "miss!"));
                    $this->draw();
                }
            }
           
            $this->assembler->reset();
        }        
    }

    public function draw() {
        // rebuild the display
        $conveyor = $this->conveyor->draw();
        $assembler = $this->assembler->draw();
        $status = $this->status->draw();

        // get the longest conveyor line
        $conveyorWidth = 0;

        foreach ($conveyor as $line) {
            if (Config::containsMb(Colors::stripColor($line))) {
                $lineWidth = 0;
            } else {
                $lineWidth = strlen(Colors::stripColor($line));
            }

            if ($lineWidth > $conveyorWidth) {
                $conveyorWidth = $lineWidth;
            }
        }

        // get the longest assembler width
        $assemblerWidth = 0;

        foreach ($assembler as $line) {
            if (Config::containsMb(Colors::stripColor($line))) {
                $lineWidth = 0;
            } else {
                $lineWidth = strlen(Colors::stripColor($line));
            }

            if ($lineWidth > $assemblerWidth) {
                $assemblerWidth = $lineWidth;
            }
        }

        // get the longest status screen
        $statusWidth = 0;

        foreach ($status as $line) {
            $lineWidth = strlen(Colors::stripColor($line));

            if ($lineWidth > $statusWidth) {
                $statusWidth = $lineWidth;
            }
        }

        // draw the conveyor and assembler into a table
        $table = Config::drawTable(array($conveyorWidth, $assemblerWidth, $statusWidth), array($conveyor, $assembler, $status));
        
        // print!
        Config::draw($table);
    }

    public function gameOver() {
		echo "GAME OVER\n\n";

        // quit the game
		system("stty " . $this->term);

		exit;
	}
}