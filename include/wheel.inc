<?php
class Wheel {
    private $char;
    
    public function __construct() {
        $this->char = "|";
    }

    public function tick() {
        if ($this->char == "|") {
            $this->char = "/";
        } else if ($this->char == "/") {
            $this->char = "-";
        } else if ($this->char == "-") {
            $this->char = "\\";
        } else if ($this->char == "\\") {
            $this->char = "|";
        }
    }

    public function __toString() {
        return $this->char;
    }
}