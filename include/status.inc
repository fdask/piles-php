<?php
class Status {
    private $score;
    private $msgs;
    private $misses;
    private $hits;

    public function __construct() {
        $this->score = 0;
        $this->misses = 0;
        $this->hits = 0;
        $this->msgs = array();
    }

    public function tick() {
        
    }

    public function addMsg($msg) {
        $this->msgs[] = $msg;
    }

    public function addScore($s) {
        $this->score += $s;
    }

    public function addMiss() {
        $this->misses++;
    }

    public function addHit() {
        $this->hits++;
    }

    public function draw() {
        $ret = array();

        $ret[] = "score: " . $this->score;
        $ret[] = "hits: " . $this->hits;
        $ret[] = "misses: " . $this->misses;

        $msgs = array_slice($this->msgs, -3, 3, true);

        foreach ($msgs as $msg) {
            $ret[] = $msg;
        }

        return $ret;
    }
}