<?php
class Assembler {
    private $buildSize;
    private $pile;
    private $complete;

    private static $alnum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    public function __construct() {
        $this->reset();
    }

    public function tick() {
        if ($this->pile->getSize() < $this->buildSize) {
            // add a block
            $this->addChar();
        } else {
            $this->complete = true;
        }
    }

    public function addChar() {
        // char to add
        $bits = str_split($this::$alnum);
        $c = rand(0, count($bits) - 1);

        $this->pile->addChar($bits[$c]);
    }

    public function getPile() {
        return $this->pile;
    }

    public function reset() {
        $this->complete = false;
        $this->pile = new Pile();
        $this->buildSize = rand(2, Config::$maxPileHeight);
    }

    public function isDone() {
        return $this->complete;
    }

    public function draw() {
        $ret = array();

        $chars = $this->pile->getChars();

        //print_r($chars);
        if ($chars) {
            for ($x = Config::$maxPileHeight - 1; $x >= 0; $x--) {
                $ret[$x] = "  " . array_pop($chars) . "  ";
            }
        }

        $ret[Config::$maxPileHeight] = " === ";
        $ret[Config::$maxPileHeight + 1] = "  X  ";
        
        return $ret;
    }
}