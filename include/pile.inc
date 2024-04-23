<?php
class Pile {
    private $behaviors;

    // for each character, we have a counter-behaviour slot
    private $counters;

    // the items in the pile
    private $characters;

    public static $behaviorLeft = 'LEFT';
    public static $behaviorRight = 'RIGHT';
    public static $behaviorUp = 'UP';
    public static $behaviorDown = 'DOWN';

    public function __construct() {
        $this->behaviors = array();
        $this->characters = array();
        $this->counters = array();

        // 50/50 shot for all up/down
        if (rand(0, 1)) {
            // 50/50 shot for up or down
            if (rand(0, 1)) {
                $this->behaviors[] = Pile::$behaviorUp;
            } else {
                $this->behaviors[] = Pile::$behaviorDown;
            }
        }

        // 50/50 shot for left right
        if (rand(0, 1)) {
            // 50/50 shot for left or right
            if (rand(0, 1)) {
                $this->behaviors[] = Pile::$behaviorLeft;
            } else {
                $this->behaviors[] = Pile::$behaviorRight;
            }
        }
        //$this->behaviors[] = 'SINGLERIGHT';
    }

    public function tick() {
         if (in_array(Pile::$behaviorUp, $this->behaviors) && !in_array(Pile::$behaviorUp, $this->counters)) {
            // rotate up
            $i = array_shift($this->characters);
            $this->characters[] = $i;
        }
        
        if (in_array(Pile::$behaviorDown, $this->behaviors) && !in_array(Pile::$behaviorDown, $this->counters)) {
            // rotate down
            $i = array_pop($this->characters);
            array_unshift($this->characters, $i);
        }

        if (in_array(Pile::$behaviorLeft, $this->behaviors) && !in_array(Pile::$behaviorLeft, $this->counters)) {
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

        if (in_array(Pile::$behaviorRight, $this->behaviors) && !in_array(Pile::$behaviorRight, $this->counters)) {
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

    public function isCountered() {
        foreach ($this->behaviors as $behavior) {
            if (!in_array($behavior, $this->counters)) {
                return false;
            }
        }

        return true;
    }

    public function getBehaviors() {
        return $this->behaviors;
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

        if ($c == Pile::$behaviorRight && in_array(Pile::$behaviorLeft, $this->counters)) {
            // remove the single left
            $this->removeCounter(Pile::$behaviorLeft);
            $add = false;
        }

        if ($c == Pile::$behaviorLeft && in_array(Pile::$behaviorRight, $this->counters)) {
            // remove the single right
            $this->removeCounter(Pile::$behaviorRight);
            $add = false;
        }

        if ($c == Pile::$behaviorUp && in_array(Pile::$behaviorDown, $this->counters)) {
            // remove the all down
            $this->removeCounter(Pile::$behaviorDown);
            $add = false;
        }

        if ($c == Pile::$behaviorDown && in_array(Pile::$behaviorUp, $this->counters)) {
            // remove the all up
            $this->removeCounter(Pile::$behaviorUp);
            $add = false;
        }

        // add the counter
        if ($add) {
            $this->counters[] = $c;
        }
    }
}