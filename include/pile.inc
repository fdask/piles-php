<?php
class Pile {
    private $behaviors;

    // for each character, we have a counter-behaviour slot
    private $counters;

    // the items in the pile
    private $characters;

    public function __construct() {
        $this->behaviors = array();
        $this->characters = array();
        $this->counters = array();

        // 50/50 shot for all up/down
        if (rand(0, 1)) {
            // 50/50 shot for up or down
            if (rand(0, 1)) {
                $this->behaviors[] = 'ALLUP';
            } else {
                $this->behaviors[] = 'ALLDOWN';
            }
        }

        // 50/50 shot for left right
        if (rand(0, 1)) {
            // 50/50 shot for left or right
            if (rand(0, 1)) {
                $this->behaviors[] = 'SINGLELEFT';
            } else {
                $this->behaviors[] = 'SINGLERIGHT';
            }
        }
        //$this->behaviors[] = 'SINGLERIGHT';
    }

    public function tick() {
         if (in_array("ALLUP", $this->behaviors) && !in_array("ALLUP", $this->counters)) {
            // rotate up
            $i = array_shift($this->characters);
            $this->characters[] = $i;
        }
        
        if (in_array("ALLDOWN", $this->behaviors) && !in_array("ALLDOWN", $this->counters)) {
            // rotate down
            $i = array_pop($this->characters);
            array_unshift($this->characters, $i);
        }

        if (in_array("SINGLELEFT", $this->behaviors) && !in_array("SINGLELEFT", $this->counters)) {
            // move a single left
            $charPos = rand(0, count($this->characters) - 1);
            $char = $this->characters[$charPos];

            if ($char == "A") {
                $this->characters[$charPos] = "9";
            } else if ($char == "0") {
                $this->characters[$charPos] = "Z";
            } else {
                $val = ord($char);

                $this->characters[$charPos] = chr($val - 1);
            }
        }

        if (in_array("SINGLERIGHT", $this->behaviors) && !in_array("SINGLERIGHT", $this->counters)) {
            // move a single right
            $charPos = rand(0, count($this->characters) - 1);
            $char = $this->characters[$charPos];

            if ($char == "Z") {
                $this->characters[$charPos] = "0";
            } else if ($char == "9") {
                $this->characters[$charPos] = "A";
            } else {
                $val = ord($char);

                $this->characters[$charPos] = chr($val + 1);
            }
        }
    }

    public function addChar($c) {
        $this->characters[] = $c;
    }

    public function getChars() {
        return $this->characters;
    }

    public function getSize() {
        return count($this->characters);
    }

    public function removeCounter($c) {
        $newCounters = array();

        for ($x = 0; $x < count($this->counters); $x++) {
            if ($this->counters[$x] != $c) {
                $newCounters[] = $this->counters[$x];
            }
        }

        $this->counters = $newCounters;
    }

    public function getCounters() {
        return $this->counters;
    }

    public function addCounter($c) {
        $add = true;

        if ($c == "SINGLERIGHT" && in_array("SINGLELEFT", $this->counters)) {
            // remove the single left
            $this->removeCounter("SINGLELEFT");
            $add = false;
        }

        if ($c == "SINGLELEFT" && in_array("SINGLERIGHT", $this->counters)) {
            // remove the single right
            $this->removeCounter("SINGLERIGHT");
            $add = false;
        }

        if ($c == "ALLUP" && in_array("ALLDOWN", $this->counters)) {
            // remove the all down
            $this->removeCounter("ALLDOWN");
            $add = false;
        }

        if ($c == "ALLDOWN" && in_array("ALLUP", $this->counters)) {
            // remove the all up
            $this->removeCounter("ALLUP");
            $add = false;
        }

        // add the counter
        if ($add) {
            $this->counters[] = $c;
        }
    }
}

abstract class Behavior {

}