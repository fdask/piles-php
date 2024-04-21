<?php
class Wheel {
    private $char;
    
    public function __construct($char = null) {
        if (is_null($char)) {
            $this->char = "|";
        } else {
            $this->char = $char;
        }
    }

    public function tick() {
        if ($this->char == "/") {
            $this->char = "|";
        } else if ($this->char == "-") {
            $this->char = "/";
        } else if ($this->char == "\\") {
            $this->char = "-";
        } else if ($this->char == "|") {
            $this->char = "\\";
        }
    }

    public function draw() {
        return $this->char;
    }
}