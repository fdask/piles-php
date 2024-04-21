<?php
class Pile {
    private $behaviors;
    private $characters;

    public function __construct() {
        $this->characters = array();
        $this->behaviours = array();
    }

    public function tick() {

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
}