<?php
class Conveyor {
    private $wheel1;
    private $wheel2;

    // how much space we have
    private $length;
    
    // how many ticks before advance
    private $speed; 
    
    // where the current count is
    private $advanceTicker;

    // whats on the conveyor (size of length)
    private $contents; 

    // integer, which pile in the length
    private $playerX;

    // which height?
    private $playerY;

    public function __construct() {
        $this->wheel1 = new Wheel();
        $this->wheel2 = new Wheel("-");

        $this->advanceTicker = 0;
        $this->playerX = 0;
        $this->playerY = 0;
        $this->speed = 1;
        $this->length = Config::$defaultConveyorLength;
        
        $this->contents = array();

        for ($x = 0; $x < $this->length; $x++) {
            $this->contents[$x] = null;
        }
    }

    public function countPiles() {
        $count = 0;

        foreach ($this->contents as $c) {
            if (!is_null($c)) {
                $count++;
            }
        }

        return $count;
    }

    public function addPile(Pile $p) {
        if ($this->countPiles() + 1 >= $this->length) {
            $pop = array_pop($this->contents);
        } else {
            $pop = null;
        }

        array_unshift($this->contents, $p);

        return $pop;
    }

    public function moveLeft() {
        if ($this->playerX < $this->length - 1) {
            $this->playerX++;
        }
    }

    public function moveRight() {
        if ($this->playerX > 0) {
            $this->playerX--;
        }
    }

    public function moveUp() {
        if ($this->playerY > 0) {
            $this->playerY--;
        } else {
            $this->playerY = Config::$maxPileHeight - 1;
        }
    }

    public function moveDown() {
        if ($this->playerY < Config::$maxPileHeight - 1) {
            $this->playerY++;
        } else {
            $this->playerY = 0;
        }
    }

    public function advance() {
        
    }

    public function tick() {
        $this->advanceTicker++;

        if ($this->advanceTicker == $this->speed) {
            $this->advanceTicker = 0;
            $this->advance();
            
            $this->wheel1->tick();
            $this->wheel2->tick();

            // tick the piles
            for ($x = 0; $x < $this->length; $x++) {
                if (!is_null($this->contents[$x])) {
                     $this->contents[$x]->tick();
                }
            }
        }
    }

    public function draw() {
        $ret = array();

        for ($x = $this->length - 1; $x >= 0; $x--) {
            // for each slot on the conveyor
            // if there is a pile, get it
            // otherwise make a blank
            if (is_null($this->contents[$x])) {
                $chars = array();

                for ($i = 0; $i < Config::$maxPileHeight; $i++) {
                    $chars[$i] = " ";
                }
            } else {
                $chars = $this->contents[$x]->getChars();
            }

            if (!isset($ret[Config::$maxPileHeight + 1])) {
                $ret[Config::$maxPileHeight + 1] = " ";
            }

            if (!isset($ret[Config::$maxPileHeight])) {
                $ret[Config::$maxPileHeight] = $this->wheel1->draw();
            }

            if ($x == $this->length - 1) {               
                // on the first go through
                for ($y = Config::$maxPileHeight - 1; $y >= 0; $y--) {
                    // if the line doesn't exist yet in the return array
                    // add a blank one
                    if (!isset($ret[$y])) {
                        $ret[$y] = " ";
                    }

                    // add item from the chars array
                    if (!empty($chars)) {
                        if ($y == $this->playerY && $this->playerX == $x) {
                            $ret[$y] .= Colors::fg_color('white', "[" . array_pop($chars) . "]");
                        } else {
                            $ret[$y] .= " " . array_pop($chars) . " ";
                        }
                    } else {
                        if ($this->playerY == $y && $this->playerX == $x) {
                            $ret[$y] .= Colors::fg_color('white', "[ ]");
                        } else {
                            $ret[$y] .= "   ";
                        }
                    }
                }

                $ret[Config::$maxPileHeight] .= "===";
                $ret[Config::$maxPileHeight + 1] .= " ";
                
                if ($this->playerX == $x) {
                    $ret[Config::$maxPileHeight + 1] .= Colors::fg_color('white', $x);
                } else {
                    $ret[Config::$maxPileHeight + 1] .= $x;
                }

                $ret[Config::$maxPileHeight + 1] .= " ";
            } else {
                // every other go through
                for ($y = Config::$maxPileHeight - 1; $y >= 0; $y--) {
                    if (!empty($chars)) {
                        if ($y == $this->playerY && $this->playerX == $x) {
                            $ret[$y] .= " " . Colors::fg_color('white', "[" . array_pop($chars) . "]");
                        } else {
                            $ret[$y] .= "  " . array_pop($chars) . " ";
                        }
                    } else {
                        if ($this->playerY == $y && $this->playerX == $x) {
                            $ret[$y] .= Colors::fg_color('white', " [ ]");
                        } else {
                            $ret[$y] .= "    ";
                        }
                    }
                }
                
                $ret[Config::$maxPileHeight] .= "-===";
                $ret[Config::$maxPileHeight + 1] .= "  ";

                if ($this->playerX == $x) {
                    $ret[Config::$maxPileHeight + 1] .= Colors::fg_color('white', $x);
                } else {
                    $ret[Config::$maxPileHeight + 1] .= $x;
                }

                $ret[Config::$maxPileHeight + 1] .= " ";
            }
        }
        
        if (Config::$debug) {
            $ret[Config::$maxPileHeight + 3] = "player x: " . $this->playerX . " y: " . $this->playerY;
            $ret[Config::$maxPileHeight + 4] = "advanceTicker: " . $this->advanceTicker . " speed: " . $this->speed;
        }

        return $ret;
    }
}