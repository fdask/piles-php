<?php
class Config {
    public static $debug = true;
    public static $defaultConveyorLength = 2;
	public static $tickLen = 4000;
    public static $maxPileHeight = 5;
    public static $screenWidth = 100;
    
    // scoring
    public static $leftRightScore = 15;
    public static $upDownScore = 10;

	public static function autoload($class) {
        $file = __DIR__ . "/" . str_replace('\\', '/', strtolower($class)) . ".inc";
        
        if (file_exists($file)) {
            require $file;
        } else {
            if (Config::$debug) {
                debug_print_backtrace();
            }
            
            die("Couldnt find file: $file\n");
        }
    }

    public static function toTable($data) {
        $cols = count($data[0]);

        $charsPerCol = floor(Config::$screenWidth / $cols) - $cols;

        $rows = count($data);

        $ret = "";

        for ($row = 0; $row < $rows; $row++) {
            $r = $data[$row];

            for ($col = 0; $col < $cols; $col++) {
                if ($col > 0) {
                    $ret .= "|";
                }

                $c = $r[$col];

                if (strlen(Colors::stripColor($c)) > $charsPerCol) {
                    $ret .= substr($c, 0, $charsPerCol);
                } else if (strlen(Colors::stripColor($c)) == $charsPerCol) {
                    $ret .= $c;
                } else if (strlen(Colors::stripColor($c)) < $charsPerCol) {
                    $ret .= str_pad($c, $charsPerCol, " ");
                }
            }

            $ret .= "\n";
        }
        
        return $ret;
    }

	public static function waitForInput() {
        $input = '';

        $read = [STDIN];
        $write = null;
        $except = null;

        readline_callback_handler_install('', function() {});

        // Read characters from the command line one at a time until there aren't any more to read
        do {
            $input .= fgetc(STDIN);
        } while (stream_select($read, $write, $except, 0, 1));

        readline_callback_handler_remove();

        return $input;
    }

    public static function keyPressEnter() {
        return chr(10);
    }

    public static function keypressShiftUpArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(65);
    }

    public static function keypressUpArrow() {
        return chr(27) . chr(91) . chr(65);
    }

    public static function keypressDownArrow() {
        return chr(27) . chr(91) . chr(66);
    }

    public static function keypressShiftDownArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(66);
    }

    public static function keypressLeftArrow() {
        return chr(27) . chr(91) . chr(68);
    }

    public static function keypressShiftLeftArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(68);
    }

    public static function keypressRightArrow() {
        return chr(27) . chr(91) . chr(67);
    }

    public static function keypressShiftRightArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(67);
    }

	public static function arrowLeft() {
		return json_decode('"\u2190"');
	}

	public static function arrowRight() {
		return json_decode('"\u2192"');
	}

	public static function arrowUp() {
		return json_decode('"\u2191"');
	}

	public static function arrowDown() {
		return json_decode('"\u2193"');
	}

    public static function draw($display) {
        if (!Config::$debug) {
            system("clear");
        }

        for ($x = 0; $x < count($display); $x++) {
            if (isset($display[$x])) {
                echo $display[$x];
            }
             
            echo "\n";
        }

        echo "\n";

        if (Config::$debug) {
            echo "memory " . memory_get_usage() . " bytes\n";
        }
    }

    public static function drawTable($widths, $data) {
        $ret = array();
        
        // figure out the total numbers of lines
        $lines = 0;

        foreach ($data as $d) {
            if (count($d) > $lines) {
                $lines = count($d);
            }
        }

        for ($line = 0; $line <= $lines; $line++) {
            $curStr = "";

            foreach ($data as $count => $d) {
                if (isset($d[$line])) {
                    $str = Colors::stripColor($d[$line]);

                    if (Config::containsMb($str)) {
                        if (mb_strlen($str) <= $widths[$count]) {          
                            $print = str_pad($d[$line], $widths[$count], " ") . "|";
                        } else {
                            $print = mb_substr($d[$line], 0, $widths[$count]) . "|";
                        }
                    } else {
                        if (strlen($str) <= $widths[$count]) {
                            $c = $widths[$count] - strlen($str);

                            $print = $d[$line];

                            for ($cc = 0; $cc < $c; $cc++) {
                                $print .= " ";
                            }

                            $print .= "|";
                        } else {
                            $print = substr($d[$line], 0, $widths[$count]) . "|";
                        }
                    }
                } else {
                    $print = str_pad(" ", $widths[$count]) . "|";
                }

                $curStr .= $print;
            }
            
            $ret[] = $curStr;
        }

        return $ret;
    }

    public static function containsMb($str) {
        return !mb_check_encoding($str, 'ASCII') && mb_check_encoding($str, 'UTF-8');
    }
}

spl_autoload_register("Config::autoload");