<?php
class Game {
	private $constructor;
    private $assembler;
    private $conveyor;
    
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
                case Config::keypressDownArrow():
                    $this->conveyor->moveDown();
                
                    $this->draw();

                    break;
                case Config::keypressLeftArrow():
                    $this->conveyor->moveLeft();

                    $this->draw();

                    break;
                case Config::keypressRightArrow():
                    $this->conveyor->moveRight();

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

        // check if the assembler is completed
        if ($this->assembler->isDone()) {
            $pile = $this->assembler->getPile();
            $this->conveyor->addPile($pile);
           
            $this->assembler->reset();
        }        
    }

    public function draw() {
        // rebuild the display
        $conveyor = $this->conveyor->draw();
        $assembler = $this->assembler->draw();

        // get the longest conveyor line
        $conveyorWidth = 0;

        foreach ($conveyor as $line) {
            $lineWidth = strlen(Colors::stripColor($line));

            if ($lineWidth > $conveyorWidth) {
                $conveyorWidth = $lineWidth;
            }
        }

        // get the longest assembler width
        $assemblerWidth = 0;

        foreach ($assembler as $line) {
            $lineWidth = strlen(Colors::stripColor($line));

            if ($lineWidth > $assemblerWidth) {
                $assemblerWidth = $lineWidth;
            }
        }

        // draw the conveyor and assembler into a table
        $table = Config::drawTable(array($conveyorWidth, $assemblerWidth), array($conveyor, $assembler));
        
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